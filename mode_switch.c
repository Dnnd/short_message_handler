#include <stdio.h>
#include <states.h>
#include <msp430.h>

void ConfigureSwitchModeButton(void)
{
    P1IE |= 0x03;
    P1IES &= 0x03;
}

#pragma vector=PORT1_VECTOR
__interrupt void OnButtonClickHandler(void)
{

    unsigned ifg = P1IFG;
    printf("got %u ifg button, current state: %u\n", ifg, STATE_CONTEXT.state);
    switch (ifg)
    {
    case 0x01:
        if (STATE_CONTEXT.state == AUDIO_STOPPED)
        {
            STATE_CONTEXT.command = SWITCH_TO_PLAYBACK;
        }
        else
        {
            STATE_CONTEXT.command = PAUSE;
        }
        break;
    case 0x02:
        if (STATE_CONTEXT.state == AUDIO_STOPPED)
        {
            STATE_CONTEXT.command = SWITCH_TO_RECORDING;
        }
        else
        {
            STATE_CONTEXT.command = PAUSE;
        }
        break;
    }
    P1IFG = 0;
    __bic_SR_register_on_exit(LPM0_bits);
}
