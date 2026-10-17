#ifndef MMIO_GPIO_H
#define MMIO_GPIO_H

#include <stdbool.h>
#include <stdint.h>

/* Bounded training driver for classic ESP32 GPIO18/19/21/22/23 only. */
typedef enum {
    MMIO_GPIO_OK = 0,
    MMIO_GPIO_INVALID_PIN,
    MMIO_GPIO_INVALID_ARGUMENT
} mmio_gpio_result_t;

typedef enum {
    MMIO_GPIO_PULL_NONE = 0,
    MMIO_GPIO_PULL_UP,
    MMIO_GPIO_PULL_DOWN
} mmio_gpio_pull_t;

bool mmio_gpio_supported(uint8_t pin);
mmio_gpio_result_t mmio_gpio_config_output(uint8_t pin, bool initial_high);
mmio_gpio_result_t mmio_gpio_config_input(uint8_t pin, mmio_gpio_pull_t pull);
mmio_gpio_result_t mmio_gpio_write(uint8_t pin, bool high);
mmio_gpio_result_t mmio_gpio_read(uint8_t pin, bool *high);
mmio_gpio_result_t mmio_gpio_deinit(uint8_t pin);

#endif
