cmake_minimum_required(VERSION 3.25)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Angelscript REQUIRED) # Angelscript::angelscript
find_package(asio REQUIRED) # asio::asio
find_package(jsoncons REQUIRED) # jsoncons
find_package(RmlUi REQUIRED) # RmlLua RmlCore RmlDebugger
find_package(RocksDB REQUIRED) # RocksDB::rocksdb
# find_package(sol2 REQUIRED) # sol2
find_package(Threads REQUIRED) # Threads::Threads

add_library(libraries INTERFACE)
target_link_libraries(libraries
    INTERFACE
        Angelscript::angelscript
        asio::asio
        jsoncons
        RocksDB::rocksdb
        Threads::Threads
)

add_library(scripts STATIC)
target_link_libraries(scripts
    PUBLIC
        libraries
)
target_sources(scripts
    PUBLIC
        scripts/scripts_ext.h scripts/scripts_ext.cpp
)

set(EXE_NAME untitled)
add_executable(${EXE_NAME} WIN32)

target_sources(${EXE_NAME}
    PRIVATE
        main.cpp
)

target_link_libraries(${EXE_NAME}
    PRIVATE
        RmlCore RmlDebugger
        libraries
        scripts
)

set(SCRIPT_EXE script)
add_executable(${SCRIPT_EXE})
target_link_libraries(${SCRIPT_EXE}
    PRIVATE
        libraries
        scripts
)

target_sources(${SCRIPT_EXE}
    PRIVATE
        script.cpp
)