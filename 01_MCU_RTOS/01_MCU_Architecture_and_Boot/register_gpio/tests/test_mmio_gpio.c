#include "mmio_gpio.h"
#include "mmio_port.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define GPIO_BASE UINT32_C(0x3FF44000)
#define MUX18 UINT32_C(0x3FF49070)
#define OUT_SET (GPIO_BASE + UINT32_C(0x08))
#define OUT_CLR (GPIO_BASE + UINT32_C(0x0C))
#define ENABLE_SET (GPIO_BASE + UINT32_C(0x24))
#define ENABLE_CLR (GPIO_BASE + UINT32_C(0x28))
#define IN_REG (GPIO_BASE + UINT32_C(0x3C))
#define MATRIX18 (GPIO_BASE + UINT32_C(0x578))

typedef struct {
    uintptr_t address;
    uint32_t value;
} fake_reg_t;

static fake_reg_t regs[64];
static size_t reg_count;
static fake_reg_t writes[64];
static size_t write_count;

static uint32_t *slot(uintptr_t address)
{
    for (size_t i = 0; i < reg_count; ++i) {
        if (regs[i].address == address) {
            return &regs[i].value;
        }
    }
    assert(reg_count < 64);
    regs[reg_count].address = address;
    regs[reg_count].value = 0;
    return &regs[reg_count++].value;
}

uint32_t mmio_port_read32(uintptr_t address)
{
    return *slot(address);
}

void mmio_port_write32(uintptr_t address, uint32_t value)
{
    assert(write_count < 64);
    writes[write_count++] = (fake_reg_t){address, value};
    *slot(address) = value;
}

static void reset_fake(void)
{
    reg_count = 0;
    write_count = 0;
}

int main(void)
{
    const uint32_t bit18 = UINT32_C(1) << 18;
    assert(mmio_gpio_supported(18));
    assert(mmio_gpio_supported(19));
    assert(!mmio_gpio_supported(20));
    assert(!mmio_gpio_supported(6));

    reset_fake();
    assert(mmio_gpio_config_output(20, false) == MMIO_GPIO_INVALID_PIN);
    assert(write_count == 0);

    reset_fake();
    *slot(MUX18) = UINT32_C(0x8000) | (UINT32_C(7) << 12) |
                   (UINT32_C(1) << 8) | (UINT32_C(1) << 7);
    assert(mmio_gpio_config_output(18, false) == MMIO_GPIO_OK);
    assert(writes[0].address == ENABLE_CLR && writes[0].value == bit18);
    assert((*slot(MUX18) & (UINT32_C(7) << 12)) == (UINT32_C(2) << 12));
    assert((*slot(MUX18) & UINT32_C(0x8000)) != 0); /* unrelated bit preserved */
    assert((*slot(MUX18) & (UINT32_C(3) << 7)) == 0); /* pulls cleared */
    assert((*slot(MATRIX18) & UINT32_C(0xFFF)) == (UINT32_C(256) | (UINT32_C(1) << 10)));
    assert(writes[write_count - 2].address == OUT_CLR);
    assert(writes[write_count - 1].address == ENABLE_SET);
    assert(writes[write_count - 1].value == bit18);

    reset_fake();
    assert(mmio_gpio_write(18, true) == MMIO_GPIO_OK);
    assert(writes[0].address == OUT_SET && writes[0].value == bit18);
    assert(mmio_gpio_write(18, false) == MMIO_GPIO_OK);
    assert(writes[1].address == OUT_CLR && writes[1].value == bit18);

    reset_fake();
    assert(mmio_gpio_config_input(18, (mmio_gpio_pull_t)99) == MMIO_GPIO_INVALID_ARGUMENT);
    assert(write_count == 0);
    assert(mmio_gpio_config_input(18, MMIO_GPIO_PULL_UP) == MMIO_GPIO_OK);
    assert(writes[0].address == ENABLE_CLR && writes[0].value == bit18);
    assert((*slot(MUX18) & (UINT32_C(1) << 9)) != 0); /* input enabled */
    assert((*slot(MUX18) & (UINT32_C(1) << 8)) != 0); /* pull-up */
    bool high = false;
    assert(mmio_gpio_read(18, NULL) == MMIO_GPIO_INVALID_ARGUMENT);
    *slot(IN_REG) = bit18;
    assert(mmio_gpio_read(18, &high) == MMIO_GPIO_OK && high);

    reset_fake();
    assert(mmio_gpio_deinit(18) == MMIO_GPIO_OK);
    assert(writes[0].address == ENABLE_CLR && writes[0].value == bit18);
    assert(writes[1].address == OUT_CLR && writes[1].value == bit18);
    puts("MMIO GPIO host tests: PASS");
    return 0;
}
