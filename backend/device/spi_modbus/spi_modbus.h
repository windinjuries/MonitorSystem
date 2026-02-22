
#ifndef __SPI_MODBUS_H__
#define __SPI_MODBUS_H__

#ifdef __cplusplus
extern "C" {
#endif  

extern int spi_modbus_init();

extern int spi_modbus_xfer(int fd, char *tx_buffer, int tx_len, char *rx_buffer, int *rx_len);

extern int spi_modbus_poll();

#ifdef __cplusplus
}   
#endif

#endif  // __SPI_MODBUS_H__
