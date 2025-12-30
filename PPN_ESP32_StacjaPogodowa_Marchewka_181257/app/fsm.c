#include "fsm.h"
#include "esp_log.h"

#define TAG "FSM"

static system_state_t state = STATE_INIT;


// Initialize the FSM
void fsm_init(void)
{
    ESP_LOGI(TAG, "FSM init");
    state = STATE_INIT;
}

// Get the current state of the FSM
system_state_t fsm_get_state()
{
    return state;
}

// Start the FSM from IDLE to RUN
void fsm_start()
{
    if (state == STATE_IDLE) {
        ESP_LOGI(TAG, "FSM START");
        state = STATE_RUN;
    }
}

// Reset the FSM to INIT state
void fsm_reset()
{
    ESP_LOGI(TAG, "FSM RESET");
    state = STATE_INIT;
}

// Set the FSM to ERROR state
void fsm_error()
{
    ESP_LOGI(TAG, "FSM ERROR");
    state = STATE_FAULT;
}

// Set the FSM to IDLE state
void fsm_idle()
{
    ESP_LOGI(TAG, "FSM IDLE");
    state = STATE_IDLE;
}