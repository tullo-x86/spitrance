# PROJECT ROOT

cmake_minimum_required (VERSION 2.6)

project (spi-led)

set (SPILED_VERSION_MAJOR 1)
set (SPILED_VERSION_MINOR 0)

# Source files
set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/spidevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ledstrip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
  
add_subdirectory("${PROJECT_SOURCE_DIR}/FastLED")
add_subdirectory("${PROJECT_SOURCE_DIR}/patterns")

add_executable(spitrance ${SOURCE} ${HEADERS})