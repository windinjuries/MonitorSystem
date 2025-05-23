/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <string.h>

#include "hlog.h"

#define SPI_DEV_PATH "/dev/spidev0.0"

#define SPI_SYS_PATH "/sys/bus/spi/devices/spi0.0"

static char log_buf[256];

// SPI config
static int fd;                  					
static unsigned  mode = SPI_MODE_0;    
static uint8_t bits = 8;        			
static uint32_t speed = 40000000; // 1Mhz
static uint16_t delay = 0;         		

void sfud_log_debug(const char *file, const long line, const char *format, ...);

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    
    sfud_err result = SFUD_SUCCESS;
    uint8_t *temp = malloc(write_size + read_size);
    if(temp == NULL)
    {
        return -1;
    }

    /**
     * add your spi write and read code
     */

    int ret = 0;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)write_buf,
        .rx_buf = (unsigned long)temp,
        .len = write_size + read_size,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
        .tx_nbits = 1,
        .rx_nbits = 1
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        LOGE("can't send spi message\n");
        return -1;
    }
    memcpy(read_buf, temp + write_size, read_size);
    return 0;
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */
    int ret = 0;

    flash->spi.wr = spi_write_read;
    flash->spi.lock = NULL;
    flash->spi.unlock = NULL;
    flash->spi.user_data = NULL;
    /* about 100 microsecond delay */
    flash->retry.delay = NULL;
    /* adout 60 seconds timeout */
    flash->retry.times = 60 * 10000;
    struct stat info;
    if (stat(SPI_SYS_PATH, &info) != 0) 
    {
        LOGE("%s not exist\n", SPI_SYS_PATH);
        return -1;
    }

    const char *command = "echo spidev > /sys/bus/spi/devices/spi0.0/driver_override";
    ret = system(command);
    if (ret == -1) 
    {
        LOGE("%s command execute fail\n", command);
        return -1;
    }
    command = "echo spi0.0 > /sys/bus/spi/drivers/spidev/bind";
    ret = system(command);
    if (ret == -1) 
    {
        LOGE("%s command execute fail\n", command);
        return -1;
    }

    fd = open(SPI_DEV_PATH, O_RDWR);
    if (fd < 0)
    {
        LOGE("can't open %s\n", SPI_DEV_PATH);
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
    {
        LOGE("can't set spi mode\n");
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
    {
        LOGE("can't set spi mode\n");
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        LOGE("can't set bits per word\n");
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        LOGE("can't set bits per word\n");
        return -1;
    }
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        LOGE("can't set max speed hz\n");
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        LOGE("can't set max speed hz\n");
        return -1;
    }
    return 0;
}
/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\r\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\r\n", log_buf);
    va_end(args);
}
