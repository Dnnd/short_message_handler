#include <context.h>

void HandleStateTransfer(const Context*);
void StopToRecordingHandler(const Context*);
void StopToPlaybackHandler(const Context*);
void PlaybackToStopHandler(const Context*);
void RecordingToStopHanlder(const Context*);
void StopToPauseHandler(const Context*);
void RecordingToOVFHandler(const Context*);
void PlaybackToEOFHandler(const Context*);
void StopToResetHandler(const Context*);
void StopToStopHandler(const Context*);
void RecordingToRecrodingHandler(const Context*);
void OVFToStopHandler(const Context*);
void EOFToStopHandler(const Context*);
