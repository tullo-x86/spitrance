cmake_minimum_required (VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

SET(CMAKE_C_FLAGS 			"-static-libgcc -static-libstdc++ -static -O0 -g")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS 	"-static-libgcc -static-libstdc++ -static -O0 -g")
SET(CMAKE_CXX_FLAGS 			"-static-libgcc -static-libstdc++ -std=gnu++11 -static -O0 -g")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static-libgcc -static-libstdc++ -std=gnu++11 -static -O0 -g")

add_subdirectory(src)
