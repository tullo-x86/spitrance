#!/bin/bash

#cd build

echo "Passing configuration to CMake"
cmake -Bbuild -Hsrc -DCMAKE_TOOLCHAIN_FILE=../build-setup/armhf.cmake

echo "Now making..."
make -C build
