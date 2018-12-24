#ifndef WAIT_H_
#define WAIT_H_
#include "msp430fg4618.h"
#include "math.h"
#include <stdio.h>
#define WAIT_CCR TBCCR6
#define WAIT_CCR_CTL TBCCTL6
#define WAIT_CCR_IV TBIV_TBCCR6

struct WaitCCRConfig
{
    unsigned frequency;
    unsigned _clock_per_ms;
    unsigned _ms_threshold;
};

struct WaitCCRConfig wait_ccr_config = { };

void SetWaitCCRFrequency(unsigned frequency_kHz)
{
    wait_ccr_config.frequency = frequency_kHz;
    wait_ccr_config._clock_per_ms = frequency_kHz ;
    wait_ccr_config._ms_threshold = 0xFFFF / wait_ccr_config._clock_per_ms;
    printf("freq, kHz: %u\n, cpm: %u\n, mt: %u\n", wait_ccr_config.frequency, wait_ccr_config._clock_per_ms, wait_ccr_config._ms_threshold);
}


void Wait(unsigned ms)
{
    printf("have %u\n", TBR);
    WAIT_CCR_CTL &= ~CCIE;
    if (wait_ccr_config._ms_threshold < ms)
    {
        ms = wait_ccr_config._ms_threshold;
    }
    WAIT_CCR = TBR + ms * wait_ccr_config._clock_per_ms;
    WAIT_CCR_CTL = CCIE;
    __bis_SR_register(LPM0_bits + GIE);
    printf("become %u\n", TBR);
}

#endif /* WAIT_H_ */
