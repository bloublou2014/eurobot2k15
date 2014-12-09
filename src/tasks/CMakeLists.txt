#set(SRC_FILES
#    ${SRC_FILES}
#	${CMAKE_CURRENT_SOURCE_DIR}/AbstractTask.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/TestTask.cpp
#    PARENT_SCOPE
#)

add_library(tasks AbstractTask.cpp TestTask.cpp)

target_link_libraries(tasks core messages)
