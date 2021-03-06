#ifndef WAIT_H_
#define WAIT_H_
#include <msp430.h>
#include <math.h>
#include <wait.h>
#include <debug.h>

struct WaitCCRConfig wait_ccr_config = { .frequency = 0, ._clock_per_ms = 0,
        ._clock_per_mcs = 0, ._mcs_threshold = 0, ._ms_threshold = 0 };

void SetWaitCCRFrequency(unsigned frequency_kHz) {
    wait_ccr_config.frequency = frequency_kHz;
    wait_ccr_config._clock_per_ms = frequency_kHz;
    wait_ccr_config._ms_threshold = 0xFFFF / wait_ccr_config._clock_per_ms;

    wait_ccr_config._clock_per_mcs = ceil(frequency_kHz / 1000.);
    wait_ccr_config._mcs_threshold = 0xFFFF / wait_ccr_config._clock_per_mcs;
    DEBUGF("freq, kHz: %u\n, cpm: %u\n, mt: %u\n, cpmc: %u\n, mct: %u\n",
            wait_ccr_config.frequency, wait_ccr_config._clock_per_ms,
            wait_ccr_config._ms_threshold, wait_ccr_config._clock_per_mcs,
            wait_ccr_config._mcs_threshold);
}

void Wait(unsigned ms) {
    __disable_interrupt();
    WAIT_CCR_CTL &= ~CCIE;
    unsigned current = TBR;
    if (wait_ccr_config._ms_threshold < ms) {
        ms = wait_ccr_config._ms_threshold;
    }
    WAIT_CCR = current + ms * wait_ccr_config._clock_per_ms;
    WAIT_CCR_CTL |= CCIE;
    __bis_SR_register(LPM0_bits + GIE);
    WAIT_CCR_CTL &= ~CCIE;
}

void WaitMCS(unsigned mcs) {
    __disable_interrupt();
    WAIT_CCR_CTL &= CCIE;
    unsigned current = TBR;
    if (wait_ccr_config._mcs_threshold < mcs) {
        mcs = wait_ccr_config._mcs_threshold;
    }
    WAIT_CCR = current + mcs * wait_ccr_config._clock_per_mcs;
    WAIT_CCR_CTL |= CCIE;
    __bis_SR_register(LPM0_bits + GIE);
    WAIT_CCR_CTL &= ~CCIE;
}

#endif /* WAIT_H_ */
