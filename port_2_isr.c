#include <context.h>
#include <msp430.h>
#include <debug.h>
#include <port_2_isr.h>
#include <isd4002.h>

void ConfigurePort2ISR() {
    P2IE |= BIT3 + BIT0;
    P2IES |= BIT3 + BIT0;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2ISR() {

    unsigned ifg = P2IFG;
    const Context *ctx = GetGlobalContext();
    if (ifg & BIT0) {
        const RACHandlerContext *rac_ctx = GetRACHandlerContext();
        DEBUGF("RAC: wait_flag(%d),record_raw(%d), play_raw(%d) \n",rac_ctx->wait_flag,ctx->record_row, ctx->play_row);
        if (RACInterruptHandler(ctx)) {
            P2IFG &= ~BIT0;
            __bic_SR_register_on_exit(LPM0_bits);
            return;
        }
        P2IFG &= ~BIT0;
    } else if (ifg & BIT3) {
        DEBUGF("INTERRUPT\n");
        if (INTInterruptHandler(ctx)) {
            P2IFG &= ~BIT3;
            return;
        }
        P2IFG &= ~BIT3;
    }

}
