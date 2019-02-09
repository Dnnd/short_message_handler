#include <stdint.h>
#include <stddef.h>
#include <msp430.h>
#include <debug.h>

enum {
    SPI_READ_TX = 0x02, SPI_WRITE_TX = 0x04, SPI_NO_TX = 0x00,
};

typedef struct {
    uint8_t state;
} SpiState;

SpiState spi_state = { .state = SPI_NO_TX };

void SendSPI(uint8_t* data_chunks, size_t data_len) {
    size_t i;
    for (i = 0; i < data_len; ++i) {
        while (!(IFG2 & UCB0TXIFG))
            ;
        UCB0TXBUF = data_chunks[i];
    }
}

void StartTransactionSPI() {
    P4OUT &= ~BIT1;
}

void EndTransactionSPI() {
    P4OUT |= BIT1;
}

void ReadSPI(uint8_t* data_chunks, size_t data_len) {
    size_t i;
    for (i = 0; i < data_len; ++i) {
        while (!(IFG2 & UCB0RXIFG))
            ;
        data_chunks[i] = UCB0RXBUF;
        DEBUGF("read %d of %d\n", i + 1, data_len);
    }
}

void SetupDMASendSPIData(uint8_t* data_chunks, size_t data_len) {
    DMA1SAL = (__SFR_FARPTR) data_chunks;
    DMA1DAL = (__SFR_FARPTR) &UCB0TXBUF;
    DMA1SZ = data_len;
}

void SetupDMAReceiveSPIData(uint8_t* data_chunks, size_t data_len) {
    DMA2DAL = (__SFR_FARPTR) data_chunks;
    DMA2SAL = (__SFR_FARPTR) &UCB0RXBUF;
    DMA2SZ = data_len;
}

void SetupDMASendSPIConfiguration() {
    DMACTL0 |= DMA1TSEL_13;
    DMA1CTL = DMADT_0 + DMASRCINCR_3 + DMADSTINCR_0 + DMAIE + DMASRCBYTE
            + DMADSTBYTE;
    IE2 &= ~UCB0TXIE;
}

void SetupDMAReceiveSPIConfiguration() {
    DMACTL0 |= DMA2TSEL_12;
    DMA2CTL = DMADT_0 + DMASRCINCR_0 + DMADSTINCR_3 + DMAIE + DMASRCBYTE
            + DMADSTBYTE;
    IE2 &= ~UCB0RXIE;
}

void SendSPIWithDMA(uint8_t* data_chunks, size_t data_len) {
    SetupDMASendSPIData(data_chunks, data_len);
    spi_state.state |= SPI_WRITE_TX;
    P4OUT &= ~BIT1;
    __disable_interrupt();
    DMA1CTL |= DMAEN;
    IFG2 &= ~UCB0TXIFG;
    IFG2 |= UCB0TXIFG;
    __bis_SR_register(LPM0_bits + GIE);
    P4OUT |= BIT1;
}

void ReadSPIWithDMA(uint8_t* data_chunks, size_t data_len) {
    SetupDMAReceiveSPIData(data_chunks, data_len);
    spi_state.state |= SPI_READ_TX;
    __disable_interrupt();
    P4OUT &= ~BIT1;
    DMA2CTL |= DMAEN;
    IFG2 &= ~UCB0RXIFG;
    IFG2 |= UCB0RXIFG;
    __bis_SR_register(LPM0_bits + GIE);
    P4OUT |= BIT1;
}

void ReadWriteSPIWithDMA(uint8_t *source_buff, size_t source_buff_size,
        uint8_t *dst_buff, size_t dest_buff_size) {
    SetupDMASendSPIData(source_buff, source_buff_size);
    SetupDMAReceiveSPIData(dst_buff, dest_buff_size);
    spi_state.state |= SPI_READ_TX + SPI_WRITE_TX;
    __disable_interrupt();
    P4OUT &= ~BIT1;
    IFG2 &= ~UCB0RXIFG;
    IFG2 &= ~UCB0TXIFG;
    IFG2 |= UCB0RXIFG + UCB0TXIFG;
    DMA1CTL |= DMAEN;
    DMA2CTL |= DMAEN;
    __bis_SR_register(LPM0_bits + GIE);
    P4OUT |= BIT1;
}

#pragma vector=DMA_VECTOR
__interrupt void SpiTransferISR() {
    unsigned ifg = DMAIV;
    switch (ifg) {
    case DMAIV_DMA1IFG:
        spi_state.state &= ~SPI_WRITE_TX;
        if (spi_state.state == SPI_NO_TX) {
            __bic_SR_register_on_exit(LPM0_bits);
            return;
        }
        break;
    case DMAIV_DMA2IFG:
        spi_state.state &= ~SPI_READ_TX;
        if (spi_state.state == SPI_NO_TX) {
            __bic_SR_register_on_exit(LPM0_bits);
            return;
        }
        break;
    default:
        break;
    }
}
