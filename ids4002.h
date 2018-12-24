#ifndef IDS4002_H_
#define IDS4002_H_
#include <stdint.h>

#include "wait.h"

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



uint8_t IDSCommands[10][2] = { { 0, 4 }, /* POWERUP */
                               { 0, 7 }, /* SETPLAY */
                               { 0, 15 }, /* PLAY */
                               { 0, 5 }, /* SETREC */
                               { 0, 13 }, /* REC */
                               { 0, 23 }, /* SETMC */
                               { 0, 31 }, /* MC */
                               { 0, 12 }, /* STOP */
                               { 0, 8 }, /* STOPPWRDN */
                               { 0, 12 }, /* RINT */
};

IDSCommand current_command_label;

uint8_t current_command_buff[2];
uint8_t* current_command = current_command_buff;
uint8_t idx = 0;

void SendCommand(IDSCommand command)
{
    uint8_t *c = IDSCommands[command];
    current_command[0] = c[0];
    current_command[1] = c[1];
    current_command_label = command;
    IE2 |= UCB0TXIE;
    __bis_SR_register(LPM0_bits + GIE);
}

void SendCommandWithOperand(IDSCommand command, uint16_t operand)
{
    uint8_t *c = IDSCommands[command];
    current_command[0] = c[0] & (operand & 0xFF00);
    current_command[1] = c[1] & (operand & 0x00FF);
    current_command_label = command;
    IE2 |= UCB0TXIE;
    __bis_SR_register(LPM0_bits + GIE);
}

void WaitTPUD(int cycles)
{
    Wait(cycles * TPUD_MS);
}

void WaitTSTOP()
{
    Wait(TSTOP);
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0_TX_ISR(void)
{
    if (current_command == NULL)    {

        return;
    }

    if (IFG2 & UCA0TXIFG)    {
        UCB0TXBUF = current_command[idx];
        if (idx == 1)
        {
            P4OUT &= ~BIT1;
            IE2 &= ~UCB0TXIE;
            current_command = NULL;
            idx = 0;
            while (UCB0STAT & UCBUSY)
                ;
            printf("spi sent done \n");
            P4OUT |= BIT1;
            __bic_SR_register_on_exit(CPUOFF);
        } else {
            idx = 1;
        }
    }
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0_RX_ISR(void) {
    printf("got %u by spi\n", UCB0RXBUF);
}

void SwitchToRecordMode()
{
    printf("to recording \n");
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommand(POWERUP);
    WaitTPUD(2);
    SendCommand(REC);
    printf("to recording done \n");
}

void SwitchToRecordModeFromAddress(uint16_t addr)
{
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommand(POWERUP);
    WaitTPUD(2);
    SendCommandWithOperand(SETREC, addr << 5);
}

void SwitchToPlaybackMode()
{

    printf("to playback \n");
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommand(PLAY);
    printf("to playback done \n");
}

void SwitchToPlaybackModeFromAddress(uint16_t addr)
{
    SendCommand(POWERUP);
    WaitTPUD(1);
    SendCommandWithOperand(SETPLAY, addr << 5);
}

void Pause()
{
    printf("pause \n");
    SendCommand(STOP);
    WaitTSTOP();
    printf("pause done \n");
}

#endif /* IDS4002_H_ */
