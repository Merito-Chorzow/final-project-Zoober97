#pragma once

extern int error_count;
typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_RUN,
    STATE_FAULT,
    STATE_SAFE
} system_state_t;

void fsm_init();

system_state_t fsm_get_state();

void fsm_start();
void fsm_reset();
void fsm_error();
void fsm_idle();