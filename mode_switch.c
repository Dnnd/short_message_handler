#include <context.h>
#include <msp430.h>
#include <debug.h>

void ConfigureSwitchModeButton() {
    P1IE |= 0x03;
    P1IES &= 0x03;
}

#pragma vector=PORT1_VECTOR
__interrupt void OnButtonClickHandler() {

    unsigned ifg = P1IFG;
    const Context *ctx = GetGlobalContext();
    DEBUGF("got %u ifg (button), current state: %u\n", ifg, ctx->state);
    switch (ifg) {
    case 0x01:
        if (ctx->state == AUDIO_STOPPED) {
            SetTransfer(STOP_TO_PLAYBACK);
        } else if (ctx->state == AUDIO_RECORDING) {
            SetTransfer(RECORDING_TO_STOP);
        } else if (ctx->state == AUDIO_PLAYBACK) {
            SetTransfer(PLAYBACK_TO_STOP);
        }
        break;
    case 0x02:
        if (ctx->state == AUDIO_STOPPED) {
            SetTransfer(STOP_TO_RECORDING);
        } else if (ctx->state == AUDIO_RECORDING) {
            SetTransfer(RECORDING_TO_STOP);
        } else if (ctx->state == AUDIO_PLAYBACK) {
            SetTransfer(PLAYBACK_TO_STOP);
        }
        break;
    }
    P1IFG = 0;
    __bic_SR_register_on_exit(LPM0_bits);
}
