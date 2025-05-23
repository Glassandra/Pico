#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>
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

  char name[16];

  while (true) {
    printf("Name? ");
    if (scanf("%31s", name) == 1) {
      lcd_clear();
      lcd_send_command(LCD_RETURNHOME);
      lcd_print_lines("Hello ", name);
      printf("Hello %s! \n", name);
    } else {
      printf("Error\n");
     //while (getchar() != '\n' && getchar() ! EOF);
    }
    sleep_ms(500);
  }
}