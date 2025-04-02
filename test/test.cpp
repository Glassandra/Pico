#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"




int main()
{
    stdio_init_all();
    char c;
    int status = 1;

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Example to turn on the Pico W LED

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, status);
        printf("Status is: %d\n", status);
        sleep_ms(1000);
        c = getchar_timeout_us(0);
        if (c == '0') {
            status = 0;
        }
        else if (c == '1') {
            status = 1;
        }
    }
}
