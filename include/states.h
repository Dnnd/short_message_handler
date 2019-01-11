#ifndef STATES_H_
#define STATES_H_
#include <stdint.h>
typedef enum
{
    AUDIO_RECORDING,
    AUDIO_PLAYBACK,
    AUDIO_OVERFLOW,
    AUDIO_END_OF_MESSAGE,
    AUDIO_STOPPED,
} State;

typedef enum
{
    SWITCH_TO_RECORDING,
    SWITCH_TO_PLAYBACK,
    PAUSE,
    INDICATE_OVF,
    INDICATE_EOM,
    RESET,
    COMMAND_NOOP
} Command;

typedef struct
{
    State state;
    Command command;
    uint16_t operand;
} Context;

extern Context STATE_CONTEXT;

void AcceptCommand(void);

#endif /* STATES_H_ */
