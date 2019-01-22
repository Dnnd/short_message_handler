#ifndef AUDIO_OUTPUT_H_
#define AUDIO_OUTPUT_H_

extern volatile const int sinwave[];
extern volatile unsigned int sin_idx;

inline void AudioOutputCallback();

inline void EnableAudioOutput();

inline void DisableAudioOutput();

inline void EnableAudioInput();

inline void DisableAudioInput();

void EnableAudioOutputDMA();
void DisableAudioOutputDMA();
void ConfigureAudioOutputDMA();

#endif /* AUDIO_OUTPUT_H_ */
