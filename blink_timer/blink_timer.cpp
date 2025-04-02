// #include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_PIN 14
#define BUT_PIN 16
#define LED_PIN_ALT 10

bool timer_callback(struct repeating_timer *rt)
{
    bool led_on = gpio_get(LED_PIN);
    gpio_put(LED_PIN, !led_on);
    gpio_put(LED_PIN_ALT, led_on);
    return 1;
}

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    gpio_init(BUT_PIN);
    gpio_set_dir(BUT_PIN, GPIO_IN);
    gpio_pull_up(BUT_PIN);

    gpio_init(LED_PIN_ALT);
    gpio_set_dir(LED_PIN_ALT, GPIO_OUT);
    gpio_put(LED_PIN_ALT, 1);

    struct repeating_timer timer;
    add_repeating_timer_ms(500, timer_callback, NULL, &timer);

    while (true)
    {

        sleep_ms(200);
    }
}