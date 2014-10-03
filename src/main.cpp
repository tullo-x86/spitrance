/* Hello World program */

#include <stdio.h>
#include "spidevice.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

main()
{
    printf("Hello World\n");
    
    SpiDevice *spi = new SpiDevice("/dev/spidev1.0", 500, 10000000, 8);
    
    uint8_t ledBuffer[] = {
        0xff, 0xff, 0xff, 0xff, // START word
        
        0xff, 0x10, 0x00, 0x10, // One dim cyan pixel
        0xff, 0x00, 0x10, 0x10, // One dim magenta pixel
        0xff, 0x10, 0x10, 0x00, // One dim yellow pixel
        
        0x00, 0x00, 0x00, 0x00  // END word
    };
    
    spi->Transfer(ledBuffer, ARRAY_SIZE(ledBuffer));
}