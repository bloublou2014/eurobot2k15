set(PLUGINS_SRC
)

add_library(plugins ${PLUGINS_SRC})

target_link_libraries(plugins core messages)
