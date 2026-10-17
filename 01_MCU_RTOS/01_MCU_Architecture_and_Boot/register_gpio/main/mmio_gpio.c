#include "mmio_gpio.h"
#include "mmio_port.h"

#include <stddef.h>

/* ESP32 TRM Chapter 6; ESP-IDF 6.1 soc/esp32/register/soc/{reg_base,gpio,io_mux}_reg.h. */
#define GPIO_BASE              UINT32_C(0x3FF44000)
#define IO_MUX_BASE            UINT32_C(0x3FF49000)
#define GPIO_OUT_W1TS          (GPIO_BASE + UINT32_C(0x0008))
#define GPIO_OUT_W1TC          (GPIO_BASE + UINT32_C(0x000C))
#define GPIO_ENABLE_W1TS       (GPIO_BASE + UINT32_C(0x0024))
#define GPIO_ENABLE_W1TC       (GPIO_BASE + UINT32_C(0x0028))
#define GPIO_IN                (GPIO_BASE + UINT32_C(0x003C))
#define GPIO_PIN_CFG(pin)      (GPIO_BASE + UINT32_C(0x0088) + UINT32_C(4) * (pin))
#define GPIO_FUNC_OUT(pin)     (GPIO_BASE + UINT32_C(0x0530) + UINT32_C(4) * (pin))

#define IO_MUX_FUN_PD          (UINT32_C(1) << 7)
#define IO_MUX_FUN_PU          (UINT32_C(1) << 8)
#define IO_MUX_FUN_IE          (UINT32_C(1) << 9)
#define IO_MUX_MCU_SEL_MASK    (UINT32_C(7) << 12)
#define IO_MUX_GPIO_FUNCTION   (UINT32_C(2) << 12)
#define GPIO_PAD_OPEN_DRAIN    (UINT32_C(1) << 2)
#define GPIO_OUT_SEL_MASK      UINT32_C(0x00000FFF)
#define GPIO_OUT_SEL_SOFTWARE  (UINT32_C(256) | (UINT32_C(1) << 10))

/* IO MUX register offsets from io_mux_reg.h; GPIO20 is absent. */
static uintptr_t io_mux_address(uint8_t pin)
{
    switch (pin) {
    case 18: return IO_MUX_BASE + UINT32_C(0x70);
    case 19: return IO_MUX_BASE + UINT32_C(0x74);
    case 21: return IO_MUX_BASE + UINT32_C(0x7C);
    case 22: return IO_MUX_BASE + UINT32_C(0x80);
    case 23: return IO_MUX_BASE + UINT32_C(0x8C);
    default: return (uintptr_t)0;
    }
}

bool mmio_gpio_supported(uint8_t pin)
{
    return io_mux_address(pin) != (uintptr_t)0;
}

static void configure_mux(uint8_t pin, bool input_enabled, mmio_gpio_pull_t pull)
{
    const uintptr_t address = io_mux_address(pin);
    uint32_t value = mmio_port_read32(address);
    value &= ~(IO_MUX_MCU_SEL_MASK | IO_MUX_FUN_PD | IO_MUX_FUN_PU | IO_MUX_FUN_IE);
    value |= IO_MUX_GPIO_FUNCTION;
    if (input_enabled) {
        value |= IO_MUX_FUN_IE;
    }
    if (pull == MMIO_GPIO_PULL_UP) {
        value |= IO_MUX_FUN_PU;
    } else if (pull == MMIO_GPIO_PULL_DOWN) {
        value |= IO_MUX_FUN_PD;
    }
    mmio_port_write32(address, value);
}

mmio_gpio_result_t mmio_gpio_config_output(uint8_t pin, bool initial_high)
{
    if (!mmio_gpio_supported(pin)) {
        return MMIO_GPIO_INVALID_PIN;
    }
    const uint32_t mask = UINT32_C(1) << pin;
    /* Quiesce a pin that was already driving before touching its mux/matrix. */
    mmio_port_write32(GPIO_ENABLE_W1TC, mask);
    const uintptr_t cfg = GPIO_FUNC_OUT(pin);
    uint32_t value = mmio_port_read32(cfg);
    value = (value & ~GPIO_OUT_SEL_MASK) | GPIO_OUT_SEL_SOFTWARE;
    mmio_port_write32(cfg, value);

    /* Push-pull, no pull; set output latch before enabling the pad. */
    const uintptr_t pin_cfg = GPIO_PIN_CFG(pin);
    mmio_port_write32(pin_cfg, mmio_port_read32(pin_cfg) & ~GPIO_PAD_OPEN_DRAIN);
    configure_mux(pin, false, MMIO_GPIO_PULL_NONE);
    mmio_port_write32(initial_high ? GPIO_OUT_W1TS : GPIO_OUT_W1TC, mask);
    mmio_port_write32(GPIO_ENABLE_W1TS, mask);
    return MMIO_GPIO_OK;
}

mmio_gpio_result_t mmio_gpio_config_input(uint8_t pin, mmio_gpio_pull_t pull)
{
    if (!mmio_gpio_supported(pin)) {
        return MMIO_GPIO_INVALID_PIN;
    }
    if (pull != MMIO_GPIO_PULL_NONE && pull != MMIO_GPIO_PULL_UP &&
        pull != MMIO_GPIO_PULL_DOWN) {
        return MMIO_GPIO_INVALID_ARGUMENT;
    }
    mmio_port_write32(GPIO_ENABLE_W1TC, UINT32_C(1) << pin);
    configure_mux(pin, true, pull);
    return MMIO_GPIO_OK;
}

mmio_gpio_result_t mmio_gpio_write(uint8_t pin, bool high)
{
    if (!mmio_gpio_supported(pin)) {
        return MMIO_GPIO_INVALID_PIN;
    }
    mmio_port_write32(high ? GPIO_OUT_W1TS : GPIO_OUT_W1TC, UINT32_C(1) << pin);
    return MMIO_GPIO_OK;
}

mmio_gpio_result_t mmio_gpio_read(uint8_t pin, bool *high)
{
    if (!mmio_gpio_supported(pin)) {
        return MMIO_GPIO_INVALID_PIN;
    }
    if (high == NULL) {
        return MMIO_GPIO_INVALID_ARGUMENT;
    }
    *high = (mmio_port_read32(GPIO_IN) & (UINT32_C(1) << pin)) != 0;
    return MMIO_GPIO_OK;
}

mmio_gpio_result_t mmio_gpio_deinit(uint8_t pin)
{
    if (!mmio_gpio_supported(pin)) {
        return MMIO_GPIO_INVALID_PIN;
    }
    const uint32_t mask = UINT32_C(1) << pin;
    mmio_port_write32(GPIO_ENABLE_W1TC, mask);
    mmio_port_write32(GPIO_OUT_W1TC, mask);
    configure_mux(pin, false, MMIO_GPIO_PULL_NONE);
    return MMIO_GPIO_OK;
}
