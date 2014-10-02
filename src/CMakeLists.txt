cmake_minimum_required (VERSION 2.6)

project (spi-led)

add_executable(test-leds spidevice.cpp main.cpp)

set (SPILED_VERSION_MAJOR 1)
set (SPILED_VERSION_MINOR 0)

