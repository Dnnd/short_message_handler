#include "include/audio_io.h"

#include <msp430.h>
#include <debug.h>
#include <stdint.h>

//volatile const int sinwave[] = { 2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
//        4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447, 2048, 1648, 1264, 910,
//        600, 345, 156, 39, 0, 39, 156, 345, 600, 910, 1264, 1648 };

volatile const int sinwave[] = {2.048, 2.447, 2.831, 3.185, 3.495, 3.75, 3.939, 4.056, 4.095, 4.056, 3.939, 3.75, 3.495, 3.185, 2.831, 2.447, 2.048, 1.648, 1.264, 0.91, 0.6, 0.345, 0.156, 0.039, 0.0, 0.039, 0.156, 0.345, 0.6, 0.91, 1.264, 1.648};

volatile unsigned int sin_idx = 0;

inline void AudioOutputCallback() {
    if (sin_idx >= 32) {
        sin_idx = 0;
    }
    DAC12_0DAT = sinwave[sin_idx];
    if (TBCCR1 > 0xFFFF - 128) {
        TBCCR1 = 0;
    }
    TBCCR1 += 128;
    ++sin_idx;
}

void EnableAudioOutput() {
    TBCCTL1 |= CCIE;
}

void DisableAudioOutput() {
    TBCCTL1 &= ~CCIE;
}

void EnableAudioInput() {
    ADC12CTL0 |= ENC | ADC12SC;
}

void DisableAudioInput() {
    ADC12CTL0 &= ~ENC;
    ADC12CTL0 &= ~ADC12SC;
}

void ConfigureAudioOutputDMA() {
    DMACTL0 |= DMA0TSEL_5;
    DMA0SA = (void(*)()) sinwave;
    DMA0DA = (void(*)()) &DAC12_0DAT;
    DMA0SZ = 32;
    DMA0CTL |= DMADT_6 + DMASRCINCR_3 + DMADSTINCR_0;

}

//void ConfigureAudioInputDMA() {
//    DMACTL0 |= DMA0TSEL_6;
//    DMA0SA = &ADC12MEM0;
//    DMA0DA = &
//}

void EnableAudioOutputDMA() {
    DEBUGF("conf dma out\n");
    DMA0CTL |= DMAEN;
    DAC12_0CTL |= DAC12IFG + DAC12ENC;
}

void DisableAudioOutputDMA() {
    DMA0CTL &= ~DMAEN;
}
