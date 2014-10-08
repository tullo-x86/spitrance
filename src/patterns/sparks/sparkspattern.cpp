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

#define FRAMES_BETWEEN_SPARKS 30
#define TRAIL_LENGTH 4

SparksPattern::SparksPattern(int length)
: _length(length),
_framesUntilNewSpark(0)
{
    _rgbBuffer = new CRGB[length];
    _hsvBuffer = new CHSV[length];
    
    // Clear "framebuffer"
    for(int i=0; i < _length; i++)
        _hsvBuffer[i] = CHSV(_backgroundHue, 255, 255);
    
    _sparks.push_front(Spark(HUE_MAX_RAINBOW / 3, length - 1));
    
    _framesUntilNewSpark = FRAMES_BETWEEN_SPARKS;
}

void SparksPattern::Logic()
{
    int lastSparkHead = -1;
    
    // Iterate forward from pixel 0 to pixel n
    for(auto it = _sparks.begin(); it != _sparks.end(); it++)
    {
        // - Find the position of this spark
        int startIdx = it->Position;
        
        // - If the spark is beyond the end, we still need to draw its body and tail
        //   (but obviously we can't render anything beyond the framebuffer)
        if (startIdx >= _length)
        {
            startIdx = _length - 1;
        }
        
        // - Iterate backwards until the head of the last spark
        for (int pixelIdx = startIdx; pixelIdx > lastSparkHead; pixelIdx--)
        {
            //   - Set pixel hue to curent spark's hue
            _hsvBuffer[pixelIdx].hue = it->Hue;
            
            //   - Set pixel saturation to:
            //     - (HEAD) 0
            //     - (BODY) random
            //     - (TAIL) 255
            if (pixelIdx == it->Position)
            {
                _hsvBuffer[pixelIdx].sat = 0;
            }
            else if (it->Position - pixelIdx <= TRAIL_LENGTH)
            {
                // TODO: Try a logarithmic randomness instead
                _hsvBuffer[pixelIdx].sat = rand() % 255;
            }
            else
            {
                _hsvBuffer[pixelIdx].sat = 255;
            }
        }
        
        lastSparkHead = it->Position;
        // Don't move while testing the trail animation
        it->Position++;
    }
    
    // Can only destroy a spark if:
    //   - There are at least two sparks
    //  && Spark before it has reached the end
    if (_sparks.size() > 1)
    {        
        auto it = _sparks.rbegin();
        it++;
        
        if (it->Position >= (_length - 1))
        {
            _sparks.pop_back();
        }
    }
    
    if (--_framesUntilNewSpark == 0)
    {
        _framesUntilNewSpark = FRAMES_BETWEEN_SPARKS;
        _sparks.push_front(Spark(rand() % HUE_MAX_RAINBOW));
    }
}

void SparksPattern::Render()
{
    hsv2rgb_rainbow(_hsvBuffer, _rgbBuffer, _length);
}

SparksPattern::~SparksPattern()
{
    // Not sure why it doesn't like this, but I don't really care right now.
    //delete[] _rgbBuffer;
}

