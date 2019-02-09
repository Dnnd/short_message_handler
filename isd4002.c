#include <stdint.h>
#include <wait.h>
#include <spi.h>
#include <debug.h>
#include <isd4002.h>
#include <time.h>
#include <context.h>
#include <msp430.h>
#include <transfers.h>

RACHandlerContext rac_handler_context = { .wait_flag = 0 };

const RACHandlerContext* GetRACHandlerContext() {
    return &rac_handler_context;
}

void SendCommand(ISDCommand command) {
    uint8_t current_command[2] = { 0, command << 3 };
    StartTransactionSPI();
    WaitMCS(2);
    SendSPIWithDMA(current_command, 2);
    WaitMCS(2);
    EndTransactionSPI();
    WaitMCS(2);
}

void SendCommandWithOperand(ISDCommand command, uint16_t operand) {

    uint8_t current_command[2] = { operand & 0x00FF, (command << 3)
            | ((operand & 0x0300) >> 8), };
    DEBUGF("high: %d, low: %d, command: %d, operand: %d\n", current_command[0],
            current_command[1], command, operand);
    StartTransactionSPI();
    WaitMCS(1);
    SendSPIWithDMA(current_command, 2);
    WaitMCS(1);
    EndTransactionSPI();
    WaitMCS(1);
}

void WaitTPUD(int cycles) {
    Wait(cycles * TPUD_MS);
}

void WaitTSTOP() {
    Wait(TSTOP);
}

void SwitchToRecordMode() {
    SendCommand(POWERUP);
    WaitTPUD(2);
    SendCommand(REC);
}


void WaitRAC() {
    __disable_interrupt();
    rac_handler_context.wait_flag = 1;
    __bis_SR_register(LPM0_bits + GIE);
}

void SendCommandOnRAC(ISDCommand command, uint16_t operand) {
    WaitRAC();
    SendCommandWithOperand(command, operand);
    printf("command on rac\n");
}

void JumpToPlaybackAddress(uint16_t addr) {
    SendCommandWithOperand(SETPLAY, addr);
    SendCommandOnRAC(PLAY, 0);
}

void JumpToRecordingAddress(uint16_t addr) {
    SendCommandWithOperand(SETREC, addr);
    SendCommandOnRAC(REC, 0);
}

void ToPowerDownMode() {
    SendCommand(STOPPWRDN);
    WaitTSTOP();
}

void ReadInterruptData() {
    uint8_t current_command[2] = { 0, RINT };
    SendSPI(current_command, 2);
}

void Pause() {
    SendCommand(STOP);
    WaitTSTOP();
}

uint8_t RACInterruptHandler(const Context *ctx) {
    if (ctx->state == AUDIO_PLAYBACK) {
        IncPlayRowPosition();
    } else if (ctx->state == AUDIO_RECORDING) {
        IncRecordRowPosition();
    }
    if (rac_handler_context.wait_flag != 0) {
        rac_handler_context.wait_flag = 0;
        return 1;
    }
    return 0;
}

uint8_t INTInterruptHandler(const Context *ctx) {
    if (ctx->state == AUDIO_PLAYBACK) {
        RecordingToOVFHandler(ctx);
        SetGlobalContextState(AUDIO_OVERFLOW);
    } else if (ctx->state == AUDIO_RECORDING) {
        PlaybackToEOFHandler(ctx);
        SetGlobalContextState(AUDIO_END_OF_MESSAGE);
    }
    return 0;
}
