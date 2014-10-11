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
#include <stdio.h>
#include "../../FastLED/hsv2rgb.h"

SparksPattern::SparksPattern(int length, int sparkleTrailLength, int valFalloffDistance, uint8_t valMin, uint8_t valMax, int framesBetweenSparks, int startOffset)
: _length(length),
_framesUntilNewSpark(startOffset),
_framesBetweenSparks(framesBetweenSparks),
_sparkleTrailLength(sparkleTrailLength),
_valFalloffDistance(valFalloffDistance),
_valMin(valMin),
_valMax(valMax)
{
    _rgbBuffer = new CRGB[length];
    _hsvBuffer = new CHSV[length];
    
    // Clear "framebuffer"
    for(int i=0; i < _length; i++)
        _hsvBuffer[i] = CHSV(_backgroundHue, 255, valMax);
    
    _sparks.push_front(Spark(0, length - 1));
}

void SparksPattern::Logic()
{
    
    for(Spark &spark : _sparks)
        spark.Position++;
    
    // Can only destroy a spark if:
    //   - There are at least two sparks
    //  && Spark before it has reached the end
    if (_sparks.size() > 1)
    {        
        auto spark = _sparks.rbegin();
        spark++;
        
        if (spark->Position >= (_length - 1))
        {
            _sparks.pop_back();
        }
    }
    
    if (--_framesUntilNewSpark == 0)
    {
        _framesUntilNewSpark = _framesBetweenSparks;
        _sparks.push_front(Spark(rand() % HUE_MAX_RAINBOW));
    }
}

inline int interpolate(int a, int b, int t, int range_t) {
    return ((a*t) / range_t)
           +
           (b*(range_t - t) / range_t);
}

uint8_t SparksPattern::PixelVal(int leadingSparkPosition, int pixelPosition)
{
    int distance = leadingSparkPosition - pixelPosition;
    if (distance > _valFalloffDistance)
        return _valMin;
    
    return interpolate(_valMin, _valMax, distance, _valFalloffDistance);
}


void SparksPattern::Render()
{
    int lastSparkHead = -1;
    
    // Iterate forward from pixel 0 to pixel n
    for(Spark &spark : _sparks)
    {
        // - Find the position of this spark
        int startIdx = spark.Position;
        
        // - If the spark is beyond the end, we still need to draw its body and tail
        //   (but obviously we can't render anything beyond the framebuffer)
        if (startIdx >= _length)
        {
            startIdx = _length - 1;
        }
        
        // - Iterate backwards until the head of the last spark
        for (int pixelIdx = startIdx; pixelIdx > lastSparkHead; pixelIdx--)
        {
            CHSV &pixel = _hsvBuffer[pixelIdx];
            
            //   - Set pixel hue to curent spark's hue
            pixel.hue = spark.Hue;
            
            //   - Set pixel saturation:
            //     - (HEAD) 0
            //     - (BODY) random
            //     - (TAIL) 255
            if (pixelIdx == spark.Position)
            {
                pixel.sat = 0;
            }
            else if (spark.Position - pixelIdx <= _sparkleTrailLength)
            {
                // Saturation = 100% - RandomElement({ 0xff, 0x7f, 0x3f, 0x1f, 0x0f })
                //    leaving possible sat values of { 0x00, 0x70, 0xc0, 0xe0, 0xf0 }
                pixel.sat = 0xff - (0xff >> (rand() % 5));
                //pixel.sat = interpolate(255, 0, spark.Position - pixelIdx, _sparkleTrailLength);
            }
            else
            {
                pixel.sat = 255;
            }
            
            pixel.val = PixelVal(spark.Position, pixelIdx);
        }
        
        lastSparkHead = spark.Position;
    }
    
    hsv2rgb_rainbow(_hsvBuffer, _rgbBuffer, _length);
}

SparksPattern::~SparksPattern()
{
    // Not sure why it doesn't like this, but I don't really care right now.
    //delete[] _rgbBuffer;
}

