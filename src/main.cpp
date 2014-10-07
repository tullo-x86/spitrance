/* Hello World program */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <chrono>
#include <thread>

#include "spidevice.h"
#include "FastLED/pixeltypes.h"
#include "FastLED/hsv2rgb.h"
#include "ledstrip.h"

struct Options
{
    const char *device;
    uint16_t delay;
    uint32_t speed;
    uint8_t wordLength;
    uint8_t lumi;
};

static void print_usage(const char *prog)
{
        printf("Usage: %s [-Dsbdl]\n", prog);
        puts("  -D --device   device to use (default /dev/spidev1.0)\n"
            "  -s --speed    max speed (Hz)\n"
            "  -d --delay    delay (usec)\n"
            "  -b --bpw      bits per word \n"
            "  -l --lumi     luminance\n");
        exit(1);
}

static void parse_opts(int argc, char *argv[], struct Options *options)
{
        while (1) {
                static const struct option lopts[] = {
                        { "device",  1, nullptr, 'D' },
                        { "speed",   1, nullptr, 's' },
                        { "delay",   1, nullptr, 'd' },
                        { "lumi",    1, nullptr, 'l' },
                        { NULL, 0, 0, 0 },
                };
                int c;

                c = getopt_long(argc, argv, "D:s:d:l:", lopts, NULL);

                if (c == -1)
                        break;

                switch (c) {
                case 'D':
                        options->device = optarg;
                        break;
                case 's':
                        options->speed = atoi(optarg);
                        break;
                case 'd':
                        options->delay = atoi(optarg);
                        break;
                case 'l':
                        options->lumi = (uint8_t)atoi(optarg);
                        break;
                default:
                        print_usage(argv[0]);
                        break;
                }
        }
}


#define NUM_PIXELS 4

CHSV hsvPixels[NUM_PIXELS];
CRGB rgbPixels[NUM_PIXELS];

int main(int argc, char *argv[])
{
    struct Options options;
    
    options.device = "/dev/spidev1.0";
    options.delay = 500;
    options.speed = 10000000;
    options.lumi = 31;
    
    parse_opts(argc, argv, &options);
    
    
    
    SpiDevice spi(options.device, options.delay, options.speed, 8);
    LedStrip strip(&spi, NUM_PIXELS);
    
    for(int hue=0; hue < HUE_MAX_RAINBOW * 3; hue+= 5)
    {
        fill_rainbow(rgbPixels, NUM_PIXELS, hue, 20);
        
        strip.FillGBR(rgbPixels, options.lumi);
        strip.Output();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
