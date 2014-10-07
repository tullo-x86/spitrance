/* Hello World program */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <chrono>
#include <thread>

#include "spidevice.h"
#include "FastLED/pixeltypes.h"
#include "FastLED/hsv2rgb.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

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
                        { "bpw",     1, nullptr, 'b' },
                        { "lumi",    1, nullptr, 'l' },
                        { NULL, 0, 0, 0 },
                };
                int c;

                c = getopt_long(argc, argv, "D:s:d:b:l:", lopts, NULL);

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
                case 'b':
                        options->wordLength = atoi(optarg);
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

void fillSpiBufferGBR(uint8_t spiBuffer[], CRGB rgbBuffer[], int length)
{
    for (int i=0; i < length; i++)
    {
        spiBuffer[i+4] = 0xff;
        spiBuffer[i+5] = rgbBuffer[i].g;
        spiBuffer[i+6] = rgbBuffer[i].b;
        spiBuffer[i+7] = rgbBuffer[i].r;
    }
}

int main(int argc, char *argv[])
{
    struct Options options;
    
    options.device = "/dev/spidev1.0";
    options.delay = 500;
    options.speed = 10000000;
    options.wordLength = 8;
    options.lumi = 32;
    
    parse_opts(argc, argv, &options);
    
    uint8_t lumi = options.lumi;
    
    SpiDevice *spi = new SpiDevice(options.device, options.delay, options.speed, options.wordLength);
    
    uint8_t spiBuffer[8 + (NUM_PIXELS * 4)] = {
        0x00, 0x00, 0x00, 0x00, // START frame
        
        0xff, lumi, 0x00, 0x00, // One green pixel
        0xff, 0x00, lumi, 0x00, // One blue pixel
        0xff, 0x00, 0x00, lumi, // One red pixel
        0xff, 0x00, lumi, lumi, // One magenta pixel
        
        0xff, 0xff, 0xff, 0xff, // END frame
    };
    
    spi->Open();
    
    for(int hue=0; hue < 360; hue++)
    {
        fill_rainbow(rgbPixels, NUM_PIXELS, hue);
        fillSpiBufferGBR(spiBuffer, rgbPixels, NUM_PIXELS);
        spi->Transfer(spiBuffer, ARRAY_SIZE(spiBuffer));
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    spi->Close();
}