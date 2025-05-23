#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "lcd.h"

#define PIN_LIGHT 16  // ADC0
#define PIN_WATER 17  // ADC1
#define SDA_PIN 4
#define SCL_PIN 5

int main()
{
    stdio_init_all();

    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    lcd_init();
    
    adc_init();
    adc_gpio_init(PIN_LIGHT);
    adc_gpio_init(PIN_WATER);

    char buffer[16]; 

    while (true) {
        adc_select_input(0);
        uint16_t light_value = adc_read();
        adc_select_input(1);
        uint16_t water_value = adc_read();

        printf("Light: %d, Water: %d\n", light_value, water_value);
        
        lcd_clear();
        
        sprintf(buffer, "Light: %d", light_value);
        lcd_print(buffer);
        
        lcd_set_cursor(1, 0);
        sprintf(buffer, "Water: %d", water_value);
        lcd_print(buffer);

        sleep_ms(500);
    }
}