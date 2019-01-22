#include <stdint.h>
#include <stddef.h>
#include <msp430.h>
#include <debug.h>

enum {
    SPI_READ_FG = 0x01,
    SPI_WRITE_FG = 0x02,
    SPI_WRITE_DONE = 0x08,
    SPI_READ_DONE = 0x0F,
};

uint8_t SpiState = 0;

void SendSPI(uint8_t* data_chunks, size_t data_len) {
    size_t i;
    P4OUT &= ~BIT1;
    for (i = 0; i < data_len; ++i) {
        while (!(IFG2 & UCB0TXIFG))
            ;
        UCB0TXBUF = data_chunks[i];
//        DEBUGF("sending %d of %d\n", i + 1, data_len);
    }
    P4OUT |= BIT1;
}

void ReadSPI(uint8_t* data_chunks, size_t data_len) {
    size_t i;
    P4OUT &= ~BIT1;
    for (i = 0; i < data_len; ++i) {
        while (!(IFG2 & UCB0RXIFG))
            ;
        data_chunks[i] = UCB0RXBUF;
        DEBUGF("read %d of %d\n", i + 1, data_len);
    }
    P4OUT |= BIT1;
}

void ConfigureDMASendSPI(uint8_t* data_chunks, size_t data_len) {
    DMACTL0 |= DMA1TSEL_13;
    DMA1SAL = (uintptr_t) data_chunks;
    DMA1DAL = (uintptr_t) &UCB0TXBUF;
    DMA1SZ = data_len;
    DMA1CTL = DMADT_1 + DMASRCINCR_3 + DMADSTINCR_0 + DMAIE;
    SpiState |= SPI_WRITE_FG;
}

void ConfigureDMAReceiveSPI(uint8_t* data_chunks, size_t data_len) {
    DMACTL0 |= DMA2TSEL_12;
    DMA2SAL = (uintptr_t) data_chunks;
    DMA2DAL = (uintptr_t) &UCB0RXBUF;
    DMA2SZ = data_len;
    DMA2CTL = DMADT_1 + DMASRCINCR_3 + DMADSTINCR_0 + DMAIE;
    SpiState |= SPI_READ_FG;
}

void SendSPIWithDMA(uint8_t* data_chunks, size_t data_len) {
    ConfigureDMASendSPI(data_chunks, data_len);
    P4OUT &= ~BIT1;
    DMA1CTL |= DMAEN;
    __bis_SR_register(LPM0_bits + GIE);
    P4OUT |= BIT1;
    SpiState &= ~SPI_WRITE_FG;
}

void ReadSPIWithDMA(uint8_t* data_chunks, size_t data_len) {
    ConfigureDMAReceiveSPI(data_chunks, data_len);
    P4OUT &= ~BIT1;
    DMA2CTL |= DMAEN;
    __bis_SR_register(LPM0_bits + GIE);
    P4OUT |= BIT1;
    SpiState &= ~SPI_READ_FG;
}

void ReadWriteSPIWithDMA(uint8_t *source_buff, size_t source_buff_size,
        uint8_t *dst_buff, size_t dest_buff_size) {
    ConfigureDMASendSPI(source_buff, source_buff_size);
    ConfigureDMAReceiveSPI(dst_buff, dest_buff_size);
    P4OUT &= ~BIT1;
    DMA1CTL |= DMAEN;
    DMA2CTL |= DMAEN;
    __bis_SR_register(LPM0_bits + GIE);
    P4OUT |= BIT1;
    SpiState &= ~SPI_READ_FG;
    SpiState &= ~SPI_WRITE_FG;
}

#pragma vector=DMA_VECTOR
__interrupt void SpiTransferLock() {
    unsigned ifg = DMAIV;
    DEBUGF("got dma vector interrupt\n");
    switch (ifg) {
    case DMAIV_DMA1IFG:
        SpiState |= SPI_WRITE_DONE;
        if (SpiState & (SPI_WRITE_FG + SPI_READ_FG + SPI_READ_DONE)
                || SpiState & SPI_WRITE_FG) {
            __bic_SR_register(LPM0_bits);
        }
        break;
    case DMAIV_DMA2IFG:
        SpiState |= SPI_READ_DONE;
        if (SpiState & (SPI_WRITE_FG + SPI_READ_FG + SPI_WRITE_DONE)
                || SpiState & SPI_READ_FG) {
            __bic_SR_register(LPM0_bits);
        }
        break;
    default:
        break;
    }
}
