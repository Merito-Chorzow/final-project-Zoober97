#include "cli.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "string.h"
#include "ds18b20.h"
#include "fsm.h"

#define UART_NUM UART_NUM_0
#define BUF_SIZE 128

#define TAG "CLI"

int period_ms = 1000;

// Send a message over UART
static void cli_send(const char *msg)
{
    uart_write_bytes(UART_NUM, msg, strlen(msg));
    uart_write_bytes(UART_NUM, "\r\n", 2);
}

// Process a received command
static void cli_process(char *cmd)
{
    if (strncmp(cmd, "GET TEMP", 8) == 0) {
        char buf[64];
        snprintf(buf, sizeof(buf),
                 "OK TEMP=%.2f",
                 ds18b20_read());
        cli_send(buf);
    }
    else if (strncmp(cmd, "GET AVG", 7) == 0) {
        char buf[64];
        snprintf(buf, sizeof(buf),
                 "OK AVG=%.2f",
                 avg_get());
        cli_send(buf);
    }
    else if (strncmp(cmd, "SET PERIOD", 10) == 0) {
        int p = atoi(cmd + 11);
        if (p > 0) {
            period_ms = p;
            cli_send("OK");
        } else {
            cli_send("ERR BAD_VALUE");
        }
    }
    else if (strncmp(cmd, "STAT", 4) == 0) {
        char buf[64];
        snprintf(buf, sizeof(buf),
                 "STATE=%d",
                 fsm_get_state());
        cli_send(buf);
    }
    else if (strncmp(cmd, "RESET", 5) == 0) {
        char buf[64];
        fsm_reset();
        snprintf(buf, sizeof(buf),
                 "STATE=%d",
                 fsm_get_state());
        cli_send(buf);
    }
    else {
        cli_send("ERR UNKNOWN_CMD");
    }
}

// CLI task to read commands from UART
static void cli_task(void *arg)
{
    uint8_t ch;
    char line[BUF_SIZE];
    int pos = 0;

    uart_config_t cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &cfg);

    ESP_LOGI(TAG, "CLI ready. Type command and press ENTER.");

    while (1) {
        if (uart_read_bytes(UART_NUM, &ch, 1, pdMS_TO_TICKS(100)) > 0) {

            /* ENTER = koniec komendy */
            if (ch == '\n' || ch == '\r') {
                line[pos] = 0;

                if (pos > 0) {
                    cli_process(line);
                }

                pos = 0;  // reset bufora
            }
            else {
                /* ochrona przed overflow */
                if (pos < BUF_SIZE - 1) {
                    line[pos++] = ch;
                }
            }
        }
    }
}



// Start the CLI task
void cli_start()
{
    xTaskCreate(cli_task, "cli", 4096, NULL, 5, NULL);
}
