#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_MODE 0
#define SPI_BITS_PER_WORD 8
#define SPI_SPEED 1000000 // 1MHz

// Modbus CRC-16计算
uint16_t calculate_crc(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++) 
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) 
        {
            if (crc & 0x0001) 
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else 
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int spi_modbus_init()
{
    int fd;
    // 打开SPI设备
    fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        return -1;
    }

    // 设置SPI模式
    uint8_t mode = SPI_MODE;
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        close(fd);
        return -1;
    }

    // 设置bits per word
    uint8_t bits = SPI_BITS_PER_WORD;
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        perror("Failed to set bits per word");
        close(fd);
        return -1;
    }
    return fd;

    // 设置SPI速度
    uint32_t speed = SPI_SPEED;
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("Failed to set SPI speed");
        close(fd);
        return -1;
    }

}


int spi_modbus_xfer(int fd, char *tx_buffer, int tx_len, char *rx_buffer, int *rx_len) 
{
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx_buffer,
        .rx_buf = (unsigned long)rx_buffer,
        .len = tx_len,
        .delay_usecs = 0,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS_PER_WORD,
    };

    // 发送请求
    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) 
    {
        perror("SPI send failed");
        return -1;
    }

    // 检查响应长度
    if (*rx_len < 4) return -1; // 最小Modbus响应长度

    printf("rx: ");
    for(int i = 0; i < *rx_len; i++) 
    {
        printf("%02X ", rx_buffer[i]);
    }
    return 0;
}

int spi_modbus_poll()
{
    char tx_data[256];
    char rx_data[256];
    int tx_len = 0;
    int rx_len = 256;

    int fd = 0;
    int ret = 0;

    tx_data[0] = 0x01;  // slave address
    tx_data[1] = 0x00;  // function code
    tx_data[2] = 0x00; 
    tx_data[3] = 0x00; 
    fd = spi_modbus_init();
    if (fd < 0) return -1;

    while(1)
    {
        ret = spi_modbus_xfer(fd, tx_data, tx_len, rx_data, &rx_len);
        if (ret < 0) 
        {
            printf("SPI Modbus transfer failed\n");
        }
    }
    close(fd);
    return 0;
}
