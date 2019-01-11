#include <stdint.h>
#include <stddef.h>
#include <msp430.h>

void SendSPI(uint8_t* data_chunks, size_t data_len) {
    size_t i;
    P4OUT &= ~BIT1;
    for(i=0; i < data_len; ++i) {
        while (!(IFG2 & UCB0TXIFG));
        UCB0TXBUF = data_chunks[i];
        printf("sending %d of %d\n", i + 1, data_len);
    }
    P4OUT |= BIT1;
}
