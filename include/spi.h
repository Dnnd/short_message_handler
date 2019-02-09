#ifndef SPI_H_
#define SPI_H_
#include <stdint.h>
#include <stddef.h>
void SendSPI(uint8_t* data_chunks, size_t data_len);
void ReadSPI(uint8_t* data_chunks, size_t data_len);
void SendSPIWithDMA(uint8_t* data_chunks, size_t data_len);
void ReadSPIWithDMA(uint8_t* data_chunks, size_t data_len);
void ReadWriteSPIWithDMA(uint8_t *source_buff, size_t source_buff_size,
        uint8_t *dst_buff, size_t dest_buff_size);
void SetupDMASendSPIData(uint8_t* data_chunks, size_t data_len);
void SetupDMAReceiveSPIData(uint8_t* data_chunks, size_t data_len);
void SetupDMASendSPIConfiguration();
void SetupDMAReceiveSPIConfiguration();
void StartTransactionSPI();
void EndTransactionSPI();
__interrupt void SpiTransferISR();
#endif /* SPI_H_ */
