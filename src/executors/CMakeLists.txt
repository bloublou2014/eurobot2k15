set(EXECUTORS_SRC
    ExampleExecutor.cpp
    AbstractExecutor.cpp
    #MotionExecutor.cpp
    #ModbusExecutor.cpp
    #ServosExecutor.cpp
)

add_subdirectory(msg)
add_subdirectory(servo)


add_library(executors ${EXECUTORS_SRC})

target_link_libraries(executors core messages utils drivers)
