
if(CMAKE_CROSSCOMPILING)
    message("CONFIGURING FOR CROSS COMPILE")

       
	include_directories(${TOOLCHAIN_INCLUDE})
	link_directories(${TOOLCHAIN_LIB}) 

	#boost include
    include_directories(${PROJECT_SOURCE_DIR}/libs/boost_arm)
    link_directories(${PROJECT_SOURCE_DIR}/libs/boost_arm/stage/lib)
else()
    message("CONFIGURING FOR LINUX")

    #boost include
    include_directories(${PROJECT_SOURCE_DIR}/libs/boost)
    link_directories(${PROJECT_SOURCE_DIR}/libs/boost/stage/lib)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_FILES Robot.cpp)

add_subdirectory(core)
add_subdirectory(drivers)
add_subdirectory(executors)
add_subdirectory(messages)
add_subdirectory(tasks)
add_subdirectory(plugins)
add_subdirectory(utils)

add_executable(eurobot2k15 ${SRC_FILES})

target_link_libraries(eurobot2k15 pthread)

target_link_libraries(eurobot2k15 core executors messages tasks plugins servo )

#link boost
target_link_libraries(eurobot2k15 boost_chrono boost_filesystem boost_locale boost_serialization boost_signals boost_timer boost_thread boost_system boost_regex boost_log)

#SET(CMAKE_C_COMPILER /home/fax_drive/memristor/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /home/fax_drive/memristor/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)
