/*
 * Copyright (c) 2014 Daniel Tullemans <email>
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

#include "sparkspattern.h"

#include <string.h>
#include <stdlib.h>
#include "../../FastLED/hsv2rgb.h"

SparksPattern::SparksPattern(int length)
: _length(length)
{
    _rgbBuffer = new CRGB[length];
    _hsvBuffer = new CHSV[length];
    
    memset(_rgbBuffer, 0x00, sizeof(CRGB) * length);
    memset(_hsvBuffer, 0x00, sizeof(CHSV) * length);
}

void SparksPattern::Logic()
{
    _hsvBuffer[_frame].hue = rand() % HUE_MAX_RAINBOW;
    _hsvBuffer[_frame].sat = 255;
    _hsvBuffer[_frame].val = 255;
}

void SparksPattern::Render()
{
    hsv2rgb_rainbow(_hsvBuffer, _rgbBuffer, _length);
}

SparksPattern::~SparksPattern()
{
    delete[] _rgbBuffer;
}

