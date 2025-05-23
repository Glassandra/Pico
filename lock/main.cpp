

/* Only tested on wokwi, dont have enough buttons to try on local pico*/


#include <stdio.h>
#include "pico/stdlib.h"

#define BUT_RED 11  // Reset button
#define BUT_GRE 12  // Button 1
#define BUT_BLU 13  // Button 2
#define BUT_YEL 14  // Button 3
#define BUT_BLA 15  // Button 4

#define LED_RED 17
#define LED_GRE 16

// Reset the buffer and counter
void reset(int* buffer, int* counter) {
  *counter = 0;
}

// Check if the entered code matches the password
bool checkPassword(int* buffer, int* password) {
  for (int i = 0; i < 4; i++) {
    if (buffer[i] != password[i]) {
      return false;
    }
  }
  return true;
}

int main() {
  stdio_init_all();

  int password[] = {1, 2, 3, 4};  // green, blue, yellow, black
  bool isLocked = true;
  int buffer[4] = {0};
  int counter = 0;
  
  // Button debounce variables
  bool lastButtonStates[5] = {true, true, true, true, true};
  
  // Initialize buttons
  gpio_init(BUT_RED);
  gpio_set_dir(BUT_RED, GPIO_IN);
  gpio_pull_up(BUT_RED);

  gpio_init(BUT_GRE);
  gpio_set_dir(BUT_GRE, GPIO_IN);
  gpio_pull_up(BUT_GRE);

  gpio_init(BUT_BLU);
  gpio_set_dir(BUT_BLU, GPIO_IN);
  gpio_pull_up(BUT_BLU);

  gpio_init(BUT_YEL);
  gpio_set_dir(BUT_YEL, GPIO_IN);
  gpio_pull_up(BUT_YEL);

  gpio_init(BUT_BLA);
  gpio_set_dir(BUT_BLA, GPIO_IN);
  gpio_pull_up(BUT_BLA);

  // Initialize LEDs
  gpio_init(LED_RED);
  gpio_set_dir(LED_RED, GPIO_OUT);
  gpio_put(LED_RED, 1);

  gpio_init(LED_GRE);
  gpio_set_dir(LED_GRE, GPIO_OUT);
  gpio_put(LED_GRE, 0);

  while (true) {
    // Update LED status based on lock state
    gpio_put(LED_RED, isLocked ? 1 : 0);
    gpio_put(LED_GRE, isLocked ? 0 : 1);
    
    // Check RED button (reset)
    bool currentRedState = gpio_get(BUT_RED);
    if (lastButtonStates[0] && !currentRedState) {  // Button pressed (active low)
      if (isLocked) {
        // When locked, red button resets the entered code
        reset(buffer, &counter);
        printf("Code reset\n");
      } else {
        // When unlocked, red button locks the system again
        isLocked = true;
        printf("Lock closed\n");
      }
    }
    lastButtonStates[0] = currentRedState;
    
    // Only process input if locked
    if (isLocked) {
      // Check GREEN button (1)
      bool currentGreenState = gpio_get(BUT_GRE);
      if (lastButtonStates[1] && !currentGreenState && counter < 4) {
        buffer[counter++] = 1;
        printf("Button 1 pressed, counter: %d\n", counter);
      }
      lastButtonStates[1] = currentGreenState;
      
      // Check BLUE button (2)
      bool currentBlueState = gpio_get(BUT_BLU);
      if (lastButtonStates[2] && !currentBlueState && counter < 4) {
        buffer[counter++] = 2;
        printf("Button 2 pressed, counter: %d\n", counter);
      }
      lastButtonStates[2] = currentBlueState;
      
      // Check YELLOW button (3)
      bool currentYellowState = gpio_get(BUT_YEL);
      if (lastButtonStates[3] && !currentYellowState && counter < 4) {
        buffer[counter++] = 3;
        printf("Button 3 pressed, counter: %d\n", counter);
      }
      lastButtonStates[3] = currentYellowState;
      
      // Check BLACK button (4)
      bool currentBlackState = gpio_get(BUT_BLA);
      if (lastButtonStates[4] && !currentBlackState && counter < 4) {
        buffer[counter++] = 4;
        printf("Button 4 pressed, counter: %d\n", counter);
      }
      lastButtonStates[4] = currentBlackState;
      
      // Check if all 4 digits have been entered
      if (counter == 4) {
        if (checkPassword(buffer, password)) {
          isLocked = false;
          printf("Lock opened!\n");
        } else {
          printf("Wrong code, resetting\n");
        }
        reset(buffer, &counter);
      }
    } else {
      // Check if RED button is pressed to lock again
      if (lastButtonStates[0] && !currentRedState) {
        isLocked = true;
        printf("Lock closed\n");
      }
    }
    
    sleep_ms(50);  // Debounce delay
  }
}