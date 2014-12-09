#set(SRC_FILES
#    ${SRC_FILES}
#    ${CMAKE_CURRENT_SOURCE_DIR}/Node.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/CommandProcessor.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/NotificationHandler.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/CommandSource.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/KeyValueStore.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/TaskManager.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/ExecutorManager.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/AbstractMessageHandler.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/NotificationSource.cpp
#    PARENT_SCOPE
#)
add_library(core Node.cpp CommandProcessor.cpp NotificationHandler.cpp CommandSource.cpp KeyValueStore.cpp TaskManager.cpp ExecutorManager.cpp AbstractMessageHandler.cpp NotificationSource.cpp)

target_link_libraries(core executors messages tasks)
