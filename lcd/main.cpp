//#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd.h"

#define SDA_PIN 4
#define SCL_PIN 5

int main() {
  stdio_init_all();

  i2c_init(i2c0, 100 * 1000);

  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  lcd_init();

  lcd_send_command(LCD_RETURNHOME);
  const uint32_t  char_delay = 200;

  while (true) {
    lcd_send_command(LCD_RETURNHOME);
    for (int i = 0; i < 16; i++) {
      lcd_char('.');
      sleep_ms(char_delay);
    }
    lcd_set_cursor(1,0);
    for (int i = 0; i < 16; i++) {
      lcd_char('.');
      sleep_ms(char_delay);
    }
  }
}