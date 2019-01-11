#include <ids4002.h>
#include <wait.h>
#include <states.h>
#include <stdio.h>

Context STATE_CONTEXT = {.state = AUDIO_STOPPED, .command = COMMAND_NOOP, .operand = 0 };

void AcceptCommand(void)
{
    printf("command %u\n", STATE_CONTEXT.command);
    switch (STATE_CONTEXT.command)
    {
    case PAUSE:
        Pause();
        STATE_CONTEXT.state = AUDIO_STOPPED;
        break;
    case SWITCH_TO_PLAYBACK:
        SwitchToPlaybackMode();
        STATE_CONTEXT.state = AUDIO_PLAYBACK;
        break;
    case SWITCH_TO_RECORDING:
        SwitchToRecordMode();
        STATE_CONTEXT.state = AUDIO_RECORDING;
        break;
    case COMMAND_NOOP:
        return;
    default:
        _never_executed();
    }
    STATE_CONTEXT.command = COMMAND_NOOP;
    printf("command done\n");
}

