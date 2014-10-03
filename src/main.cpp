/* Hello World program */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "spidevice.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

uint8_t lumi = 100;

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

static void parse_opts(int argc, char *argv[])
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
                        //device = optarg;
                        break;
                case 's':
                        //speed = atoi(optarg);
                        break;
                case 'd':
                        //delay = atoi(optarg);
                        break;
                case 'b':
                        //bits = atoi(optarg);
                        break;
                case 'l':
                        lumi = (uint8_t)atoi(optarg);
                        break;
                default:
                        print_usage(argv[0]);
                        break;
                }
        }
}

int main(int argc, char *argv[])
{    
    parse_opts(argc, argv);
    
    SpiDevice *spi = new SpiDevice("/dev/spidev1.0", 500, 10000000, 8);
    
    uint8_t ledBuffer[] = {
        0xff, 0xff, 0xff, 0xff, // START word
        
        0xff, lumi, 0x00, lumi, // One cyan pixel
        0xff, 0x00, lumi, lumi, // One magenta pixel
        0xff, lumi, lumi, 0x00, // One yellow pixel
        
        0x00, 0x00, 0x00, 0x00  // END word
    };
    
    spi->Transfer(ledBuffer, ARRAY_SIZE(ledBuffer));
}