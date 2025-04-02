#include "pico/stdlib.h"
#include <iostream>
#include <cstring>

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(1000);
    }

    while (true) {
        std::cout << "Enter: ";
        std::string input;
        if (std::cin >> input) {
            std::cout << "Got: " << input << std::endl;
        } else {
            std::cerr << "error" << std::endl;
        }

    }
    return 0;
}