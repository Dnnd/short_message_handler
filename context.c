#include <context.h>

Context STATE_CONTEXT = { .state = AUDIO_STOPPED, .transfer = EMPTY_TRANSFER,
        .operand = 0, .play_row = 0, .record_row = 0 };

void SetTransfer(StateTransfer transfer) {
    STATE_CONTEXT.transfer = transfer;
}

void SetGlobalContextState(State state) {
    STATE_CONTEXT.state = state;
}

const Context* GetGlobalContext() {
    return &STATE_CONTEXT;
}

void SetRecordRowPosition(uint16_t position) {
    STATE_CONTEXT.record_row = position;
}

void IncRecordRowPosition() {
    STATE_CONTEXT.record_row += 1;
}

void IncPlayRowPosition() {
    STATE_CONTEXT.play_row += 1;
}

void SetPlayRowPosition(uint16_t position) {
    STATE_CONTEXT.play_row = position;
}
