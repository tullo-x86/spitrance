# this one is important
SET(CMAKE_SYSTEM_NAME Linux)

# specify the cross compiler
# arm-linux-gnueabihf-cpp
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
#SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
#SET(CMAKE_ASM_YASM_COMPILER yasm)

SET(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ -std=gnu++11 -static -O3 -s")
SET(CMAKE_C_FLAGS "-static-libgcc -static-libstdc++ -static -O3 -s")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-static-libgcc -static-libstdc++ -static -O3 -s")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static-libgcc -static-libstdc++ -std=gnu++11 -static -O3 -s")

