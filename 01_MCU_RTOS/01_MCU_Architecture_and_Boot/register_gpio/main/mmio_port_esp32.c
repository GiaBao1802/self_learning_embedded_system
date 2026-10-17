#include "mmio_port.h"
#include "sdkconfig.h"

#if !defined(CONFIG_IDF_TARGET_ESP32)
#error "This MMIO address map is for the original ESP32 target only"
#endif

uint32_t mmio_port_read32(uintptr_t address)
{
    return *(volatile const uint32_t *)address;
}

void mmio_port_write32(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address = value;
}
