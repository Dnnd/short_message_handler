#include <context.h>
#include <msp430.h>
#include <debug.h>
#include <port_1_isr.h>

void ConfigurePort1ISR() {
    P1IE |= BIT1 + BIT0 + BIT6;
    P1IES |= BIT1 + BIT0 + BIT6;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1ISR() {

    unsigned ifg = P1IFG;
    const Context *ctx = GetGlobalContext();
    DEBUGF("got %u ifg (button), current state: %u\n", ifg, ctx->state);
    if (ifg & BIT0) {
        if (ctx->state == AUDIO_STOPPED) {
            SetTransfer(STOP_TO_PLAYBACK);
        } else if (ctx->state == AUDIO_RECORDING) {
            SetTransfer(RECORDING_TO_STOP);
        } else if (ctx->state == AUDIO_PLAYBACK) {
            SetTransfer(PLAYBACK_TO_STOP);
        }

    } else if (ifg & BIT1) {
        if (ctx->state == AUDIO_STOPPED) {
            SetTransfer(STOP_TO_RECORDING);
        } else if (ctx->state == AUDIO_RECORDING) {
            SetTransfer(RECORDING_TO_STOP);
        } else if (ctx->state == AUDIO_PLAYBACK) {
            SetTransfer(PLAYBACK_TO_STOP);
        }
    } else if (ifg & BIT6) {
        if (ctx->state == AUDIO_OVERFLOW) {
            SetTransfer(OVF_TO_STOP);
        } else if (ctx->state == AUDIO_END_OF_MESSAGE) {
            SetTransfer(EOF_TO_STOP);
        } else if (ctx->state == AUDIO_STOPPED) {
            SetTransfer(STOP_TO_STOP);
        }
    }
    P1IFG &= ~ifg;
    __bic_SR_register_on_exit(LPM0_bits);
}
