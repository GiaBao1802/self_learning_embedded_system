#ifndef MMIO_PORT_H
#define MMIO_PORT_H

#include <stdint.h>

/* Production: volatile 32-bit access. Host test: fake register bank. */
uint32_t mmio_port_read32(uintptr_t address);
void mmio_port_write32(uintptr_t address, uint32_t value);

#endif
