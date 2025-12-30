#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "fsm.h"
#include "ds18b20.h"
#include "esp_log.h"
#include "wifi_ap.h"
#include "web_server.h"

#define LED_GPIO GPIO_NUM_2
#define TAG "MAIN"
void app_main(void)
{
    fsm_init();
    ds18b20_init();
    wifi_ap_init();
    web_server_start();

    float t = ds18b20_read();
    if (isnan(t)) {
        ESP_LOGE(TAG, "DS18B20 error");
        return;
    }

    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    while (true)
    {
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(500));

        t = ds18b20_read();
        ESP_LOGI(TAG, "Temperature: %.2f °C", t);
        avg_add_sample(t);
        ESP_LOGI(TAG, "AVG Temperature: %.2f °C", avg_get());

    }
    
}
