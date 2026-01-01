#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "fsm.h"
#include "ds18b20.h"
#include "esp_log.h"
#include "wifi_ap.h"
#include "web_server.h"
#include "cli.h"

#define LED_GPIO GPIO_NUM_2
#define TAG "MAIN"
void app_main(void)
{
    fsm_init();
    ds18b20_init();
    wifi_ap_init();
    web_server_start();
    cli_start();
    fsm_start();
    float t = ds18b20_read();
    if (isnan(t)) {
        ESP_LOGE(TAG, "DS18B20 error");
        fsm_error();
        return;
    }

    // gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    while (true)
    {
    //     gpio_set_level(LED_GPIO, 1);
    //     vTaskDelay(pdMS_TO_TICKS(500));
    //     gpio_set_level(LED_GPIO, 0);

        if(fsm_get_state() == STATE_FAULT){
            vTaskDelay(pdMS_TO_TICKS(period_ms));
            continue;
        }
        fsm_idle();
        vTaskDelay(pdMS_TO_TICKS(period_ms));
        fsm_start();
        
        t = ds18b20_read();
        avg_add_sample(t);
    }
    
}
