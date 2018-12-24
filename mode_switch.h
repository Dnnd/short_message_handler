#ifndef MODE_SWITCH_H_
#define MODE_SWITCH_H_
#include <stdio.h>
#include "states.h"

void ConfigureSwitchModeButton(void)
{
    P1IE |= 0x03;
    P1IES &= 0x03;
}

#pragma vector=PORT1_VECTOR
__interrupt void OnButtonClickHandler(void)
{

    unsigned ifg = P1IFG;
    printf("got %u ifg button, current state: %u\n", ifg, ctx.state);
    switch (ifg)
    {
    case 0x01:
        if (ctx.state == AUDIO_STOPPED)
        {
            ctx.command = SWITCH_TO_PLAYBACK;
        }
        else
        {
            ctx.command = PAUSE;
        }
        break;
    case 0x02:
        if (ctx.state == AUDIO_STOPPED)
        {
            ctx.command = SWITCH_TO_RECORDING;
        }
        else
        {
            ctx.command = PAUSE;
        }
        break;
    }
    P1IFG = 0;
    __bic_SR_register_on_exit(CPUOFF);
}

#endif /* MODE_SWITCH_H_ */
