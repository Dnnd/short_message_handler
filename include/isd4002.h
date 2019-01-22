#ifndef ISD4002_H_
#define ISD4002_H_
#include <stdint.h>

#define TPUD_MS 25
#define TSTOP 25

typedef enum {
    POWERUP = 4,
    SETPLAY = 7,
    PLAY = 15,
    SETREC = 5,
    REC = 13,
    SETMC = 23,
    MC = 31,
    STOP = 12,
    STOPPWRDN = 8,
    RINT = 12,
    NOOP = 0
} ISDCommand;

typedef enum {
    PLAYBACK, RECORDING, STOPPED,
} IDS4002Mode;

__interrupt void USCIAB0_RX_ISR();

extern uint8_t IDSCommands[10];

void SendCommand(ISDCommand command);

void SendCommandWithOperand(ISDCommand command, uint16_t operand);

void WaitTPUD(int cycles);

void WaitTSTOP();

void SwitchToRecordMode();
void SwitchToRecordModeFromAddress(uint16_t addr);

void SwitchToPlaybackMode();

void SwitchToPlaybackModeFromAddress(uint16_t addr);

void Pause();

#endif /* ISD4002_H_ */
