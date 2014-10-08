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

#ifndef SPARKSPATTERN_H
#define SPARKSPATTERN_H

#include "../../ledstrip.h"
#include <list>

struct Spark;

class SparksPattern
{
public:
    SparksPattern(int length);
    ~SparksPattern();
    
    void Logic();
    void Render();
    
    const CRGB *GetRGBData() { return _rgbBuffer; }
    
private:
    int _length;
    int _framesUntilNewSpark;
    
    std::list<Spark> _sparks;
    
    CRGB *_rgbBuffer;
    CHSV *_hsvBuffer;
    
    uint8_t _backgroundHue;
};

struct Spark
{
    Spark(uint8_t h, int position = 0) : Hue(h), Position(position) { }
    uint8_t Hue;
    unsigned int Position;
};

#endif // SPARKSPATTERN_H
