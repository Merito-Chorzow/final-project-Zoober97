#include "fsm.h"


static system_state_t state = STATE_INIT;

void fsm_init()
{
    state = STATE_INIT;
}