//#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_PIN 14
#define BUT_PIN 16
#define LED_PIN_ALT 10
#define DEBOUNCE_TIME_MS 200

volatile uint32_t last_interrupt_time = 0;


void gpio_callback(uint gpio, uint32_t events) {
  uint32_t current_time = to_ms_since_boot(get_absolute_time());

  if (current_time - last_interrupt_time > DEBOUNCE_TIME_MS) {
    last_interrupt_time = current_time;
    bool led_on = gpio_get(LED_PIN);
    gpio_put(LED_PIN, !led_on);
    gpio_put(LED_PIN_ALT, led_on);
  }
}

int main() {
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

  gpio_set_irq_enabled_with_callback(
    BUT_PIN,
    GPIO_IRQ_EDGE_RISE,
    true,
    gpio_callback
  );

  while (true) {
    
    sleep_ms(200);
  }
}