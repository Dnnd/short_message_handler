#include <stdint.h>
#include <wait.h>
#include <spi.h>
#include <debug.h>
#include <isd4002.h>
#include <time.h>

void SendCommand(ISDCommand command) {
    uint8_t current_command[2] = { 0, command};
    SendSPI(current_command, 2);
}

void SendCommandWithOperand(ISDCommand command, uint16_t operand) {
    uint8_t current_command[2] = {
            operand & 0x00FF,
            command | ((operand & 0xFF00) >> 3),
    };
    SendSPI(current_command, 2);
//    uint8_t buff[2] = {0};
//    ReadSPI(buff, 2);
//    DEBUGF("0: %d, 1: %d", buff[0], buff[1]);
}

void WaitTPUD(int cycles) {
    Wait(cycles * TPUD_MS);
}

void WaitTSTOP() {
    Wait(TSTOP);
}

void SwitchToRecordMode() {

    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommand(POWERUP);
    WaitTPUD(2);
    SendCommand(REC);
}

void SwitchToRecordModeFromAddress(uint16_t addr) {
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommand(POWERUP);
    WaitTPUD(2);
    SendCommandWithOperand(SETREC, addr);
}

void SwitchToPlaybackMode() {

    DEBUGF("to playback \n");
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommand(PLAY);
    DEBUGF("to playback done \n");
}

void SwitchToPlaybackModeFromAddress(uint16_t addr) {
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommandWithOperand(SETPLAY, addr);
}

void Pause() {
    DEBUGF("pause \n");
    SendCommand(STOP);
    WaitTSTOP();
    DEBUGF("pause done \n");
}
