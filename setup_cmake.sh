#!/bin/bash

echo "Passing configuration to CMake"
cmake -DCMAKE_TOOLCHAIN_FILE=build/armhf.cmake

echo "Now making..."
make
