#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "hlog.h"
#include "modbus_crc.h"


#define SPI_DEVICE "/dev/spidev1.1"
#define SPI_MODE 0
#define SPI_BITS_PER_WORD 8
#define SPI_SPEED 1000000 // 1MHz

#define DATA_MAX_LEN 256
static uint8_t tx_data[DATA_MAX_LEN];
static uint8_t rx_data[DATA_MAX_LEN];

int spi_modbus_init()
{
    int fd;
    fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) 
    {
        perror("Failed to open SPI device");
        return -1;
    }

    uint8_t mode = SPI_MODE;
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) 
    {
        perror("Failed to set SPI mode");
        close(fd);
        return -1;
    }

    uint8_t bits = SPI_BITS_PER_WORD;
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) 
    {
        perror("Failed to set bits per word");
        close(fd);
        return -1;
    }

    uint32_t speed = SPI_SPEED;
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) 
    {
        perror("Failed to set SPI speed");
        close(fd);
        return -1;
    }
    return fd;
}


int spi_modbus_xfer(int fd, char *tx_buffer, int tx_len, char *rx_buffer, int *rx_len) 
{
    memset(rx_buffer, 0, DATA_MAX_LEN);
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx_buffer,
        .rx_buf = (unsigned long)rx_buffer,
        .len = tx_len,
        .delay_usecs = 1000000,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS_PER_WORD,
        .cs_change = 0,
        .tx_nbits = 1,
        .rx_nbits = 1
    };

    // 发送请求
    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if(ret <= 0) 
    {
        perror("SPI send failed\n");
        return -1;
    }
    *rx_len = ret;

    for(int i = 0; i < *rx_len; i++) 
    {
        printf("%02X ", rx_buffer[i]);
    }

    return 0;
}

int spi_modbus_poll()
{
    int tx_len = DATA_MAX_LEN;
    int rx_len = DATA_MAX_LEN;
    int32_t crc_error_count = 0;
    int32_t crc_cal_count = 0;

    int fd = 0;
    int ret = 0;

    fd = spi_modbus_init();
    if (fd < 0)
    {
        return -1;
    } 

    printf("SPI Modbus polling started\n");

    for(int i = 0; i < DATA_MAX_LEN - 2; i++)
    {
        tx_data[i] = 0xAA;
    }
    uint16_t crc = crc16_modbus((uint8_t *)tx_data, DATA_MAX_LEN - 2);
    printf("Calculated CRC16: 0x%04X\n", crc);
    tx_data[DATA_MAX_LEN - 2] = crc & 0xFF;
    tx_data[DATA_MAX_LEN - 1] = (crc >> 8) & 0xFF;
    crc = crc16_modbus((uint8_t *)tx_data, DATA_MAX_LEN);
    printf("Calculated CRC16: 0x%04X\n", crc);

    while(1)
    {
        ret = spi_modbus_xfer(fd, tx_data, tx_len, rx_data, &rx_len);
        if (ret < 0) 
        {
            printf("SPI Modbus transfer failed\n");
            break;
        }
        crc = crc16_modbus((uint8_t *)rx_data, DATA_MAX_LEN);
        printf("Calculated CRC16: 0x%04X\n", crc);
        if(crc != 0x0)
        {
            crc_error_count++;
        }
        crc_cal_count++;
        printf("CRC Error Count: %d / Total CRC Calculations: %d\n", crc_error_count, crc_cal_count);
        sleep(2);
    }
    close(fd);
    return 0;
}
