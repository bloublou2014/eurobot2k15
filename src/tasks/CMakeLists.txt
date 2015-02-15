set(TASKS_SRC
	AbstractTask.cpp 
	TestTask.cpp
	JavaScriptTask.cpp
)

add_library(tasks ${TASKS_SRC})

target_link_libraries(tasks core messages)
