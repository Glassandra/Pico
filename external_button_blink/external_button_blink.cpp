#include <stdio.h>
#include "pico/stdlib.h"

bool is_button_pressed(uint pin)
{
    return !gpio_get(pin);
}

void toggle(uint button, uint LED_LIST[], size_t led_count, int* counter)
{
    if (is_button_pressed(button))
    {
        for (size_t i = 0; i < led_count; i++)
        {
            if (*counter == i)
            {
                gpio_put(LED_LIST[i], 1);
            }
            else
            {
                gpio_put(LED_LIST[i], 0);
            }
        }
        *counter = (*counter + 1) % led_count;
    }
}

int main()
{
    stdio_init_all();

    uint LED[] = {17, 18, 19, 20, 21};
    const uint BUT = 16;
    const size_t LED_COUNT = sizeof(LED) / sizeof(LED[0]);
    int counter = 0;
    
    for (size_t i = 0; i < LED_COUNT; i++)
    {
        gpio_init(LED[i]);
        gpio_set_dir(LED[i], GPIO_OUT);
    }

    gpio_init(BUT);
    gpio_set_dir(BUT, GPIO_IN);
    gpio_pull_up(BUT);

    while (true)
    {
        toggle(BUT, LED, LED_COUNT, &counter);
        sleep_ms(100);
    }
}