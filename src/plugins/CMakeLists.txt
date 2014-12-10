set(PLUGINS_SRC
    WebServer.cpp
)

add_library(plugins ${PLUGINS_SRC})

target_link_libraries(plugins core messages)
