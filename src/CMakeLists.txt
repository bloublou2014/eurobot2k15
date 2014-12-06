if(CMAKE_CROSSCOMPILING)
    message("CONFIGURING FOR CROSS COMPILE")
    #todo
else()
    message("CONFIGURING FOR LINUX")

    #boost include
    include_directories(${PROJECT_SOURCE_DIR}/libs/boost)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    link_directories(${PROJECT_SOURCE_DIR}/libs/boost/stage/lib)
endif()

set(SRC_FILES Robot.cpp)

add_subdirectory(core)
add_subdirectory(drivers)
add_subdirectory(executors)
add_subdirectory(messages)
add_subdirectory(tasks)

add_executable(eurobot2k15 ${SRC_FILES})

#link boost
target_link_libraries(eurobot2k15 boost_chrono boost_filesystem boost_locale boost_serialization boost_signals boost_timer boost_thread boost_system boost_regex)
