set(EXECUTORS_SRC
    ExampleExecutor.cpp
    AbstractExecutor.cpp
    MotionExecutor.cpp
)

add_subdirectory(msg)

add_library(executors ${EXECUTORS_SRC})

target_link_libraries(executors core messages)
