set(DRIVERS_SRC)

add_subdirectory(motion)

add_library(drivers ${DRIVERS_SRC})

target_link_libraries(drivers core messages)
