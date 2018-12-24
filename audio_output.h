#ifndef AUDIO_OUTPUT_H_
#define AUDIO_OUTPUT_H_


volatile const int sinwave[] = { 82 };
volatile unsigned int sin_idx = 0;

inline void AudioOutputCallback(void) {
    if (sin_idx >= sizeof(sinwave)) {
         {
             sin_idx = 0;
         }
         DAC12_0DAT = sinwave[sin_idx];
         if (TBCCR1 > 0xFFFF - 128)
         {
             TBCCR1 = 0;
         }
         TBCCR1 += 128;
         ++sin_idx;
    }
}



#endif /* AUDIO_OUTPUT_H_ */
