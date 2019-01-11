#ifndef WAIT_M_H_
#define WAIT_M_H_
#include <msp430.h>

#define WAIT_CCR TBCCR6
#define WAIT_CCR_CTL TBCCTL6
#define WAIT_CCR_IV TBIV_TBCCR6

struct WaitCCRConfig
{
    unsigned frequency;
    unsigned _clock_per_ms;
    unsigned _ms_threshold;
};

extern struct WaitCCRConfig wait_ccr_config;

void SetWaitCCRFrequency(unsigned frequency_kHz);

void Wait(unsigned ms);

#endif /* WAIT_H_ */
