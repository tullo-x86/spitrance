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

#include "ledstrip.h"
#include <string.h> // memset
#include <math.h>

LedStrip::LedStrip(SpiDevice *spiDevice, int length)
: _spiDevice(spiDevice),
_length(length)
{
    _buffer = new uint8_t[8 + (_length * 4)];
    _spiDevice->Open();
}

LedStrip::~LedStrip()
{
    _spiDevice->Close();
}

void LedStrip::FillGBR(const CRGB pixels[], uint8_t brightness)
{
    AssignPixelsForwardGBR(pixels, _length, 0, brightness);
}

void LedStrip::AssignPixelsForwardGBR(const CRGB pixels[], int length, int offset, uint8_t brightness)
{
    for (int i=0; i < length; i++)
    {
        int bufIdx = offset + i;
        
        _buffer[4*bufIdx + 4] = 0xe0 | brightness;
        _buffer[4*bufIdx + 5] = pixels[i].g;
        _buffer[4*bufIdx + 6] = pixels[i].b;
        _buffer[4*bufIdx + 7] = pixels[i].r;
    }
}

void LedStrip::AssignPixelsReverseGBR(const CRGB pixels[], int length, int offset, uint8_t brightness)
{
    for (int i=0; i < length; i++)
    {
        int bufIdx = offset + length - i - 1;
        
        _buffer[4*bufIdx + 4] = 0xe0 | brightness;
        _buffer[4*bufIdx + 5] = pixels[i].g;
        _buffer[4*bufIdx + 6] = pixels[i].b;
        _buffer[4*bufIdx + 7] = pixels[i].r;
    }
}


void LedStrip::Output()
{
    // Header
    memset(_buffer, 0x00, 4);
    
    // Footer
    memset(_buffer + ((1 + _length) * 4), 0xff, 4);
    
    _spiDevice->Transfer(_buffer, 8 + (_length * 4));    
}
