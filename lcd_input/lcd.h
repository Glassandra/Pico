#pragma once
#include "hardware/i2c.h"

const int LCD_CLEARDISPLAY    = 0x01;
const int LCD_RETURNHOME      = 0x02;
const int LCD_ENTRYMODESET    = 0x04;
const int LCD_DISPLAYCONTROL  = 0x08;
const int LCD_FUNCTIONSET     = 0x20;

const int LCD_SETDDRAMADDR    = 0x80;

const int LCD_ENTRYLEFT       = 0x02;
const int LCD_DISPLAYON       = 0x04;
const int LCD_2LINE           = 0x08;

const int LCD_BACKLIGHT       = 0x08;
const int LCD_ENABLE_BIT      = 0x04;

#define LCD_CHARACTER 1
#define LCD_COMMAND 0

static int addr = 0x27;

void i2c_write_byte(uint8_t value) {
  i2c_write_blocking(i2c0, addr, &value, 1, false);
}

void lcd_toggle_enable(uint8_t value) {
  #define DELAY_US 600
  sleep_us(DELAY_US);
  i2c_write_byte(value | LCD_ENABLE_BIT);
  sleep_us(DELAY_US);
  i2c_write_byte(value & ~LCD_ENABLE_BIT);
  sleep_us(DELAY_US);
}

void lcd_send_byte(uint8_t value, int mode) {
  uint8_t high = mode | (value & 0xF0) |LCD_BACKLIGHT;
  uint8_t low = mode | ((value << 4) & 0xF0) |LCD_BACKLIGHT;

  i2c_write_byte(high);
  lcd_toggle_enable(high);
  i2c_write_byte(low);
  lcd_toggle_enable(low);
}

void lcd_data(uint8_t data) {
  lcd_send_byte(data, true);
}

void lcd_print(const char *str) {
  while (*str) {
    lcd_data(*str++);
  }
}


void lcd_char(char value) {
  lcd_send_byte(value, LCD_CHARACTER);
}

void lcd_send_command(uint8_t value) {
  lcd_send_byte(value, LCD_COMMAND);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
  uint8_t address = (row == 0) ? 0x00 + col : 0x40 + col;
  lcd_send_command(LCD_SETDDRAMADDR | address);
}

void lcd_clear(void) {
  lcd_send_command(LCD_CLEARDISPLAY);
  sleep_ms(2); 
}

void lcd_init(void) {
  sleep_ms(50);
  lcd_send_command(0x03);
  sleep_ms(5);
  lcd_send_command(0x03);
  sleep_us(150);
  lcd_send_command(0x03);
  sleep_ms(1);
  lcd_send_command(0x02);

  lcd_send_command(LCD_FUNCTIONSET | LCD_2LINE);

  lcd_send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);

  lcd_send_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT);

  lcd_clear();
}