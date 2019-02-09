#ifndef STATES_H_
#define STATES_H_
#include <stdint.h>
typedef enum {
    AUDIO_RECORDING,
    AUDIO_PLAYBACK,
    AUDIO_OVERFLOW,
    AUDIO_END_OF_MESSAGE,
    AUDIO_STOPPED,
} State;

typedef enum {
    STOP_TO_RECORDING,
    STOP_TO_PLAYBACK,
    RECORDING_TO_OVF,
    PLAYBACK_TO_EOF,
    PLAYBACK_TO_STOP,
    RECORDING_TO_STOP,
    STOP_TO_STOP,
    RECORDING_TO_RECORDING,
    EOF_TO_STOP,
    OVF_TO_STOP,
    EMPTY_TRANSFER
} StateTransfer;

typedef struct {
    State state;
    StateTransfer transfer;
    uint16_t operand;
    uint16_t play_row;
    uint16_t record_row;
} Context;

const Context* GetGlobalContext();

void SetTransfer(StateTransfer transfer);
void SetOperand(uint16_t operand);
void SetGlobalContextState(State state);
void SetPlayRowPosition(uint16_t play_row);
void SetRecordRowPosition(uint16_t record_row);
void IncRecordRowPosition();
void IncPlayRowPosition();

#endif /* STATES_H_ */
