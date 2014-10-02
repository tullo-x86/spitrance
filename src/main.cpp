/* Hello World program */

#include <stdio.h>
#include "someclass.h"

main()
{
    printf("Hello World\n");
    
    someclass *sc = new someclass();
    sc->foo();
}