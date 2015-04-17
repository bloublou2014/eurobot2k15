set(EXECUTORS_SRC
    ExampleExecutor.cpp
    AbstractExecutor.cpp
    #Abstract.cpp
    MotionExecutor.cpp
    AbstractLiftExecutor.cpp
    ExecutorConfig.cpp
    ExecutorCommon.cpp
)

add_subdirectory(msg)
add_subdirectory(servo)
add_subdirectory(liftleft)
add_subdirectory(liftright)
add_subdirectory(liftcenter)
add_subdirectory(popcorn)
add_subdirectory(flap)
add_subdirectory(enemyDetector)
add_subdirectory(carpet)


add_library(executors ${EXECUTORS_SRC})

target_link_libraries(executors core messages utils drivers)
