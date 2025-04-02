#include <stdio.h>
#include "pico/stdlib.h"



int main()
{   
    int LED_PIN = 26;
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
        gpio_put(LED_PIN, 0); 
        sleep_ms(500);
    }
}
