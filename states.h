#ifndef STATES_H_
#define STATES_H_
#include "ids4002.h"

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

Context ctx = {.state = AUDIO_STOPPED, .command = COMMAND_NOOP, .operand = 0 };

void AcceptCommand(void)
{
    printf("command %u\n", ctx.command);
    switch (ctx.command)
    {
    case PAUSE:
        Pause();
        ctx.state = AUDIO_STOPPED;
        break;
    case SWITCH_TO_PLAYBACK:
        SwitchToPlaybackMode();
        ctx.state = AUDIO_PLAYBACK;
        break;
    case SWITCH_TO_RECORDING:
        SwitchToRecordMode();
        ctx.state = AUDIO_RECORDING;
        break;
    case COMMAND_NOOP:
        return;
    default:
        _never_executed();
    }
    ctx.command = COMMAND_NOOP;
}

#endif /* STATES_H_ */
