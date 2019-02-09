#ifndef ISD4002_H_
#define ISD4002_H_
#include <stdint.h>
#include <context.h>
#define TPUD_MS 25
#define TSTOP 25

typedef enum {
    POWERUP = 4,
    SETPLAY = 28,
    PLAY = 30,
    SETREC = 20,
    REC = 22,
    SETMC = 29,
    MC = 31,
    STOP = 6,
    STOPPWRDN = 2,
    RINT = 6,
    NOOP = 0
} ISDCommand;

typedef struct {
    uint8_t wait_flag;
} RACHandlerContext;

void SendCommand(ISDCommand command);

void SendCommandWithOperand(ISDCommand command, uint16_t operand);

void WaitTPUD(int cycles);

void WaitTSTOP();

void ToPowerDownMode();
void ReadInterruptData();

void JumpToPlaybackAddress(uint16_t addr);
void JumpToRecordingAddress(uint16_t addr);

uint8_t RACInterruptHandler(const Context *ctx);
uint8_t INTInterruptHandler(const Context *ctx);

const RACHandlerContext* GetRACHandlerContext();

void Pause();

#endif /* ISD4002_H_ */
