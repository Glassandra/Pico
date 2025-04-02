#include <stdio.h>
#include <iostream>
#include <cstring>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(1000);
    }

    while (true) {
        std::cout << "Name? ";
        std::string name;
        if (std::cin >> name) {
            std::cout << "Hello " << name << "!" << std::endl;
        }
        else {
            std::cout << "Beep boop" << std::endl;
        }
    }
    return 0;
}
