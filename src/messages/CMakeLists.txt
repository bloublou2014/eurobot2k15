#set(SRC_FILES
#    ${SRC_FILES}
#    ${CMAKE_CURRENT_SOURCE_DIR}/Message.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/Notification.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/Command.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/CommandResponse.cpp
#    PARENT_SCOPE
#)

add_library(messages Message.cpp Notification.cpp Command.cpp CommandResponse.cpp StopMessage.cpp)

target_link_libraries(messages core)
