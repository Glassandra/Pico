cmake_minimum_required(VERSION 3.13)

# Initialize Pico SDK
include(pico_sdk_import.cmake)

project(mqtt_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add the main executable
add_executable(mqtt_test
    pico/mqtt_test.c
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(mqtt_test 1)
pico_enable_stdio_uart(mqtt_test 0)

# Add required libraries
target_link_libraries(mqtt_test
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(mqtt_test)