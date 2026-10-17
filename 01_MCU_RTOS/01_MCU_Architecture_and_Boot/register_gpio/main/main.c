#include "mmio_gpio.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN UINT8_C(18)
#define BLINK_MS 500

static const char *TAG = "register_gpio";

void app_main(void)
{
    if (mmio_gpio_config_output(LED_PIN, false) != MMIO_GPIO_OK) {
        ESP_LOGE(TAG, "GPIO init failed");
        return;
    }

    bool high = false;
    for (;;) {
        high = !high;
        if (mmio_gpio_write(LED_PIN, high) != MMIO_GPIO_OK) {
            ESP_LOGE(TAG, "GPIO write failed");
            break;
        }
        ESP_LOGI(TAG, "GPIO18 level=%d", (int)high);
        vTaskDelay(pdMS_TO_TICKS(BLINK_MS));
    }
    (void)mmio_gpio_deinit(LED_PIN);
}
