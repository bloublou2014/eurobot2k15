set(EXECUTORS_SRC
    ${EXECUTORS_SRC}
    #${CMAKE_CURRENT_SOURCE_DIR}/TimePassed.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/CountdownCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MotionCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetMotionState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MotionNotification.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetLiftState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LiftNotification.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ActuatorCommand.cpp

    PARENT_SCOPE
)
