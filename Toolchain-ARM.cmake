SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_C_COMPILER arm-bcm2708-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER arm-bcm2708-linux-gnueabi-g++)

SET(CMAKE_FIND_ROOT_PATH /home/milan/tools/rpi_crosscompile/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin)

SET(TOOLCHAIN_INCLUDE /home/milan/tools/rpi_crosscompile/rootfs/usr/include)
SET(TOOLCHAIN_LIB /home/milan/tools/rpi_crosscompile/rootfs/usr/lib)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


