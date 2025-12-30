#pragma once

typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_RUN,
    STATE_FAULT,
    STATE_SAFE
} system_state_t;

void fsm_init();