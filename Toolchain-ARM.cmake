SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_C_COMPILER ${RPI_TOOLS}/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${RPI_TOOLS}/bin/arm-linux-gnueabihf-g++)

#SET(CMAKE_C_COMPILER /home/project/RPI/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /home/project/RPI/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)

#SET(CMAKE_FIND_ROOT_PATH ${RPI_ROOTFS})

SET(CMAKE_FIND_ROOT_PATH /home/project/RPI/tools/)

#SET(TOOLCHAIN_INCLUDE /home/milan/tools/rpi_crosscompile/rootfs/usr/include)
#SET(TOOLCHAIN_LIB /home/milan/tools/rpi_crosscompile/rootfs/usr/lib)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


