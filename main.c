#include <audio_io.h>
#include <msp430.h>
#include <wait.h>
#include <mode_switch.h>
#include <context.h>
#include <transfers.h>
#include <debug.h>
#include <isd4002.h>

void initSPI() {

    P4DIR |= BIT1;
    P4OUT |= BIT1;
    P4SEL &= ~BIT1;

    P3SEL |= BIT0 + BIT1 + BIT2 + BIT3;

    IE2 &= ~UCB0RXIE;
    IE2 &= ~UCB0TXIE;

    UCB0CTL1 |= UCSWRST;

    UCB0CTL0 |=
    UCSYNC +
    UCCKPL +
    UCMST;

    UCB0CTL1 |= UCSSEL_2;

    UCB0BR0 = 0x02;
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;
}

void ConfigureDAC() {
    ADC12CTL0 |= REF2_5V + REFON;
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12IE;
}

void ConfigureADC() {
    ADC12CTL0 |= SHT0_4 | ADC12ON;
    ADC12CTL1 = SHP | ADC12SSEL_3;
    ADC12IE = 1;
    P6SEL |= 1;
}

void ConfigureTimerB() {
    TBCTL = TBSSEL_2 + MC_2 + TBCLR;
}


void main() {
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureDAC();
    ConfigureAudioOutputDMA();
    ConfigureTimerB();
    SetWaitCCRFrequency(1048);
//    ConfigureADC();

    ConfigureSwitchModeButton();
    initSPI();
    const Context *ctx = GetGlobalContext();
    while (1) {
        DEBUGF("sleeping!\n");
        __bis_SR_register(LPM0_bits + GIE);
        DEBUGF("awaken!\n");
        HandleStateTransfer(ctx);
        DEBUGF("accepted!\n");
    }
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TimerBISR() {

    unsigned interrupt_vec = TBIV;
    switch (interrupt_vec) {
    case TB0IV_TBCCR1:
        AudioOutputCallback();
        return;
    case WAIT_CCR_IV:
        __bic_SR_register_on_exit(LPM0_bits);
        return;
    default:
        _never_executed();
    }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR() {
//    DEBUGF("got %d from adc\n", ADC12MEM0);
    ADC12CTL0 |= ADC12SC;
}
