#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define PIN_LIGHT 16  // ADC0
#define PIN_WATER 17  // ADC1

int main()
{
    stdio_init_all();

    // Initialize ADC
    adc_init();
    
    // Initialize GPIO pins for ADC use
    adc_gpio_init(PIN_LIGHT);
    adc_gpio_init(PIN_WATER);

    while (true) {
        // Read light potentiometer (ADC0)
        adc_select_input(0);
        uint16_t light_value = adc_read();
        
        // Read water potentiometer (ADC1)
        adc_select_input(1);
        uint16_t water_value = adc_read();

        printf("Light: %d, Water: %d\n", light_value, water_value);
        sleep_ms(500);
    }
}