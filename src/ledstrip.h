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

#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include "spidevice.h"
#include "FastLED/pixeltypes.h"

class LedStrip
{
public:
    LedStrip(SpiDevice *spiDevice, int length);
    ~LedStrip();
    
    void FillGBR(const CRGB pixels[], uint8_t brightness = 31);
    void AssignPixelsForwardGBR(const CRGB pixels[], int length, int offset = 0, uint8_t brightness = 31);
    void AssignPixelsReverseGBR(const CRGB pixels[], int length, int offset = 0, uint8_t brightness = 31);
    
    void Output();
    
    inline int GetLength() { return _length; }
    
private:
    SpiDevice *_spiDevice;
    int _length;
    uint8_t *_buffer;
};

#endif // LEDSTRIP_H
