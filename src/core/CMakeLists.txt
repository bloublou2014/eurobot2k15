add_library(core Node.cpp CommandProcessor.cpp NotificationHandler.cpp CommandSource.cpp KeyValueStore.cpp TaskManager.cpp ExecutorManager.cpp AbstractMessageHandler.cpp NotificationSource.cpp Logger.cpp
TaskManagerInterface.cpp TaskStateChangeNotification.cpp)

target_link_libraries(core executors messages tasks)
