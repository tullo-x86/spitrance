/*
 * Copyright (c) 2014 Daniel Tullemans <tully@be-lumino.us>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "spidevice.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/spi/spidev.h>

SpiDevice::SpiDevice(const char *devicePath, uint16_t delay, uint32_t speed, uint8_t wordLength)
:
_devicePath(devicePath),
_delay(delay),
_speed(speed),
_wordLength(wordLength),
_deviceHandle(0)
{
}

void SpiDevice::Open()
{
    if(_deviceHandle == 0) {
        _deviceHandle = open(_devicePath, O_RDWR);
        if (_deviceHandle < 0)
        {
            perror("Couldn't open SPI device");
            abort();
        }
    }
}

void SpiDevice::Close()
{
    if(_deviceHandle != 0) {
        close(_deviceHandle);
    }
}

// badbadbad
uint8_t rx[32768] = {0, };

void SpiDevice::Transfer(const uint8_t buffer[], uint16_t length)
{
    
    struct spi_ioc_transfer tr;
    
    tr.tx_buf = (unsigned long)buffer;
    tr.rx_buf = (unsigned long)rx;
    tr.len = length;
    tr.speed_hz = _speed;
    tr.delay_usecs = _delay;
    tr.bits_per_word = _wordLength;
    
    int ret;
    
    ret = ioctl(_deviceHandle, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {    
        perror("Couldn't send SPI message");
        abort();
    }
}

