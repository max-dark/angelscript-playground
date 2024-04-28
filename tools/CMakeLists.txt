cmake_minimum_required(VERSION 3.25)
project(playground-lsp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Angelscript REQUIRED) # Angelscript::angelscript
find_package(Threads REQUIRED) # Threads::Threads

set(SERVER_LIBS server-deps)
add_library(${SERVER_LIBS} INTERFACE)
target_link_libraries(${SERVER_LIBS}
    INTERFACE
        Angelscript::angelscript
        jsoncpp
        microhttpd
        jsonrpccpp-common
        jsonrpccpp-server
        Threads::Threads
)

set(SERVER_EXE script-server)
add_executable(${SERVER_EXE})
target_link_libraries(${SERVER_EXE}
    PRIVATE
        ${SERVER_LIBS}
)
target_sources(${SERVER_EXE}
    PRIVATE
        script_server.cpp
)