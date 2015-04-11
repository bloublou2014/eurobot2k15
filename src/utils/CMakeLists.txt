set(UTILS_SRC)

add_subdirectory(geometry)
add_subdirectory(uart)
add_subdirectory(helper)
add_subdirectory(javascript)
add_subdirectory(rs485)
add_subdirectory(modbus)

add_library(utils ${UTILS_SRC})

target_link_libraries(utils core messages drivers)
