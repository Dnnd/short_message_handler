#ifndef IDS4002_H_
#define IDS4002_H_
#include <stdint.h>

#define TPUD_MS 25
#define TSTOP 25

typedef enum
{
    POWERUP, SETPLAY, PLAY, SETREC, REC, SETMC, MC, STOP, STOPPWRDN, RINT, NOOP
} IDSCommand;

typedef enum
{
    PLAYBACK, RECORDING, STOPPED,
} IDS4002Mode;

__interrupt void USCIAB0_RX_ISR(void);

extern uint8_t IDSCommands[10][2];

void SendCommand(IDSCommand command);

void SendCommandWithOperand(IDSCommand command, uint16_t operand);

void WaitTPUD(int cycles);

void WaitTSTOP();

void SwitchToRecordMode();
void SwitchToRecordModeFromAddress(uint16_t addr);

void SwitchToPlaybackMode();

void SwitchToPlaybackModeFromAddress(uint16_t addr);

void Pause();

#endif /* IDS4002_H_ */
