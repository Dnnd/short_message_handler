#include <ids4002.h>
#include <stdint.h>
#include <wait.h>
#include <spi.h>
#include <stdio.h>

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0_RX_ISR(void) {
    printf("got %u by spi\n", UCB0RXBUF);
}

uint8_t IDSCommands[10][2] = { { 1, 4 }, /* POWERUP */
                               { 1, 7 }, /* SETPLAY */
                               { 1, 15 }, /* PLAY */
                               { 1, 5 }, /* SETREC */
                               { 1, 13 }, /* REC */
                               { 1, 23 }, /* SETMC */
                               { 1, 31 }, /* MC */
                               { 1, 12 }, /* STOP */
                               { 1, 8 }, /* STOPPWRDN */
                               { 1, 12 }, /* RINT */
};

void SendCommand(IDSCommand command) {
      uint8_t *c = IDSCommands[command];
      uint8_t current_command[2] = {0};
      current_command[0] = c[0];
      current_command[1] = c[1];
      SendSPI(current_command, 2);
}

void SendCommandWithOperand(IDSCommand command, uint16_t operand)
{
    uint8_t *c = IDSCommands[command];
    uint8_t current_command[2] = {0};
    current_command[0] = c[0] & (operand & 0xFF00);
    current_command[1] = c[1] & (operand & 0x00FF);
    SendSPI(current_command, 2);
}

void WaitTPUD(int cycles)
{
    Wait(cycles * TPUD_MS);
}

void WaitTSTOP()
{
    Wait(TSTOP);
}


void SwitchToRecordMode()
{
    printf("to recording \n");
    printf("send powerpup \n");
    SendCommand(POWERUP);
    printf("wait tpud \n");
    WaitTPUD(1);
    printf("send powerpup \n");
    SendCommand(POWERUP);
    printf("wait tpud 2 \n");
    WaitTPUD(2);
    printf("send rec \n");
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
