#include <inttypes.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO GPIO_NUM_18
#define BLINK_HALF_PERIOD_MS 500U

static const char *TAG = "week01_gpio";

static void configure_led(void)
{
    const gpio_config_t config = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&config));
    ESP_ERROR_CHECK(gpio_set_level(LED_GPIO, 0));
}

void app_main(void)
{
    configure_led();
    ESP_LOGI(TAG, "GPIO %d configured as output", LED_GPIO);

    uint32_t level = 0U;
    while (true) {
        level ^= 1U;
        ESP_ERROR_CHECK(gpio_set_level(LED_GPIO, level));
        ESP_LOGI(TAG, "LED level=%" PRIu32, level);
        vTaskDelay(pdMS_TO_TICKS(BLINK_HALF_PERIOD_MS));
    }
}
