#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main()
{   
    char input[10];
    int status = 0;
    
    stdio_init_all();

    while (true) {
        printf("LED is " + status);
        scanf("%s, input");

        if (stcmp())

        sleep_ms(100);
    }
}
