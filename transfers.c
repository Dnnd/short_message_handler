#include <transfers.h>
#include <debug.h>
#include <isd4002.h>
#include <audio_io.h>
#include <indicator.h>
#include <wait.h>

void HandleStateTransfer(const Context *ctx) {
    DEBUGF("transfer %u\n", ctx->transfer);
    switch (ctx->transfer) {
    case PLAYBACK_TO_STOP:
        PlaybackToStopHandler(ctx);
        SetGlobalContextState(AUDIO_STOPPED);
        break;
    case STOP_TO_PLAYBACK:
        StopToPlaybackHandler(ctx);
        SetGlobalContextState(AUDIO_PLAYBACK);
        break;
    case STOP_TO_RECORDING:
        StopToRecordingHandler(ctx);
        SetGlobalContextState(AUDIO_RECORDING);
        break;
    case RECORDING_TO_OVF:
        RecordingToOVFHandler(ctx);
        SetGlobalContextState(AUDIO_OVERFLOW);
        break;
    case PLAYBACK_TO_EOF:
        PlaybackToEOFHandler(ctx);
        SetGlobalContextState(AUDIO_END_OF_MESSAGE);
        break;
    case RECORDING_TO_STOP:
        RecordingToStopHanlder(ctx);
        SetGlobalContextState(AUDIO_STOPPED);
        break;
    case STOP_TO_STOP:
        StopToStopHandler(ctx);
        SetGlobalContextState(AUDIO_STOPPED);
        break;
    case OVF_TO_STOP:
        OVFToStopHandler(ctx);
        SetGlobalContextState(AUDIO_STOPPED);
        break;
    case EOF_TO_STOP:
        EOFToStopHandler(ctx);
        SetGlobalContextState(AUDIO_STOPPED);
    case EMPTY_TRANSFER:
        break;
    default:
        _never_executed();
    }
    SetTransfer(EMPTY_TRANSFER);
    DEBUGF("command done\n");
}

void RecordingToStopHanlder(const Context *ctx) {
    DEBUGF("recording -> stop\n");
    DisableAudioOutput();
    Pause();
}

void StopToRecordingHandler(const Context *ctx) {
    DEBUGF("stop -> recording\n");
    EnableAudioOutput();
    SwitchToRecordModeFromAddress(ctx->record_row);
}

void PlaybackToStopHandler(const Context *ctx) {
    DEBUGF("playback -> stop\n");
    Pause();
//    DisableAudioInput();
}

void StopToPlaybackHandler(const Context *ctx) {
    DEBUGF("stop -> playback %d\n", ctx->play_row);
    SwitchToPlaybackModeFromAddress(ctx->play_row);
//    EnableAudioInput();
}

void RecordingToOVFHandler(const Context *ctx) {
    DEBUGF("ovf\n");
    ShowOverflowIndicator();
}

void PlaybackToEOFHandler(const Context *ctx) {
    DEBUGF("eof\n");
    ShowEndOfMessageIndicator();
}

void EOFToStopHandler(const Context *ctx) {
    DEBUGF("reset eof\n");
    Pause();
    SetPlayRowPosition(0);
    HideEndOfMessageIndicator();
}

void OVFToStopHandler(const Context *ctx) {
    DEBUGF("reset ovf\n");
    Pause();
    SetRecordRowPosition(0);
    HideOverflowIndicator();
}

void StopToStopHandler(const Context *ctx) {
    DEBUGF("stop -> stop\n");
}

void RecordingToRecoringHandler(const Context *ctx) {
    AudioOutputCallback();
}
