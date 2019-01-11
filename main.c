#include <msp430.h>
#include <stdio.h>
#include <wait.h>
#include <audio_output.h>
#include <mode_switch.h>
#include <states.h>

void initSPI()
{

    P4DIR |= BIT1;
    P4OUT |= BIT1;
    P4SEL &= ~BIT1;


    P3SEL |= BIT0 + BIT1 + BIT2 + BIT3;
//
    IE2 |= UCB0RXIE;
    IE2 |= UCB0TXIE;
    // Reset state machine
    UCB0CTL1 |= UCSWRST;

    // SPI + PolarityHigh + 4 PInt/STE active low + MasterSelect
    UCB0CTL0 |=
    UCSYNC +
    UCCKPL +
    UCMODE_2 +
    UCMST;
    //ACLK
    UCB0CTL1 |= UCSSEL_1;

    // /2
    UCB0BR0 = 0x02;
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;
}

void ConfigureDAC(void)
{
    ADC12CTL0 = REFON;
    DAC12_0CTL = DAC12IR + DAC12AMP_0 + DAC12ENC;
    DAC12_0DAT = 0;
}

void ConfigureTimerB(void)
{
    TBCTL = TBSSEL_1 + MC_2 + TBCLR;
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureDAC();
    ConfigureTimerB();
    SetWaitCCRFrequency(32);
    ConfigureSwitchModeButton();
    initSPI();
    while (1)
    {
        printf("sleeping!\n");
        __bis_SR_register(LPM0_bits + GIE);
        printf("awaken!\n");
        AcceptCommand();
        printf("accepted!\n");
    }
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TimerBISR(void)
{

    unsigned interrupt_vec = TBIV;
    switch (interrupt_vec)
    {
    case TB0IV_TBCCR1:
        AudioOutputCallback();
        break;
    case WAIT_CCR_IV:
        __bic_SR_register_on_exit(LPM0_bits);
        return;
    default:
        _never_executed();
    }
}
