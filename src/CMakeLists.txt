if(CMAKE_CROSSCOMPILING)
    message("CONFIGURING FOR CROSS COMPILE")

    add_definitions(-DCROSS_COMPILING)

    include_directories(${TOOLCHAIN_INCLUDE})
    link_directories(${TOOLCHAIN_LIB})
    #include wiringPi
    include_directories(/home/project/RPI/rootfs/usr/local/include)
    link_directories(/home/project/RPI/rootfs/usr/local/lib)


    #boost include
    include_directories(${PROJECT_SOURCE_DIR}/libs/boost_arm)
    link_directories(${PROJECT_SOURCE_DIR}/libs/boost_arm/stage/lib)

    #v8 include
    include_directories(/home/project/libs/v8)
    link_directories(/home/project/libs/v8/out/arm.release/obj.target/tools/gyp)
    link_directories(/home/project/libs/v8/out/arm.release/obj.target/third_party/icu)
else()
    message("CONFIGURING FOR LINUX")

    #boost include
    include_directories(${PROJECT_SOURCE_DIR}/libs/boost)
    link_directories(${PROJECT_SOURCE_DIR}/libs/boost/stage/lib)

    #v8 include
    include_directories(/home/milan/tools/v8)
    link_directories(/home/milan/tools/v8/out/x64.debug/obj.target/tools/gyp)
    link_directories(/home/milan/tools/v8/out/x64.debug/obj.target/third_party/icu)
endif()

if(${ROBOT} MATCHES VECI)
    add_definitions(-DVELIKI_ROBOT)
    message("Configuring for VELIKI")
elseif(${ROBOT} MATCHES MANJI)
    add_definitions(-DMALI_ROBOT)
    message("Configuring for MALI")
else()
    message("ERROR robot must be VECI or MANJI")
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

target_link_libraries(eurobot2k15 core executors messages tasks plugins)

#link boost
target_link_libraries(eurobot2k15 boost_chrono boost_filesystem boost_locale boost_serialization boost_signals boost_timer boost_thread boost_system boost_regex boost_log)
#link v8
set(V8_LIB -Wl,--start-group libv8_base.a libv8_libbase.a libv8_snapshot.a libv8_libplatform.a libicuuc.a libicui18n.a libicudata.a -Wl,--end-group)
target_link_libraries(eurobot2k15 rt ${V8_LIB})
#link blinki library
if(CMAKE_CROSSCOMPILING)
    target_link_libraries(eurobot2k15 wiringPi)
endif()
