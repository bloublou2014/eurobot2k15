set(SERVO_SRC
    
)

add_subdirectory(ServoTest)


add_library(servo ${SERVO_SRC})

target_link_libraries( servo executors core messages utils drivers)
