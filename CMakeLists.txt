cmake_minimum_required(VERSION 3.25)
project(playground)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS system window graphics REQUIRED)
find_package(Angelscript REQUIRED) # Angelscript::angelscript
find_package(Threads REQUIRED) # Threads::Threads

add_library(lib_sfml INTERFACE)
target_link_libraries(lib_sfml
    INTERFACE
        sfml-system
        sfml-window
        sfml-graphics
        Threads::Threads
)

if (WIN32)
    target_link_libraries(lib_sfml
        INTERFACE
            sfml-main
    )
endif ()


add_library(libraries INTERFACE)
target_link_libraries(libraries
    INTERFACE
        Angelscript::angelscript
        Threads::Threads
)

add_library(scripts STATIC)
target_link_libraries(scripts
    PUBLIC
        libraries
        lib_sfml
)
target_sources(scripts
    PUBLIC
        scripts/scripts_ext.h scripts/scripts_ext.cpp
        scripts/scripts_tools.h scripts/scripts_tools.cpp
        scripts/scripts_sfml.h scripts/scripts_sfml.cpp
        scripts/sfml_time.h scripts/sfml_time.cpp
        scripts/sfml_clock.h scripts/sfml_clock.cpp
        scripts/sfml_vectors.h scripts/sfml_vectors.cpp
        scripts/sfml_string.h scripts/sfml_string.cpp
)

set(EXE_NAME untitled)
add_executable(${EXE_NAME} WIN32)

target_sources(${EXE_NAME}
    PRIVATE
        main.cpp
)

target_link_libraries(${EXE_NAME}
    PRIVATE
        libraries
        scripts
)

set(SCRIPT_EXE script)
add_executable(${SCRIPT_EXE})
target_link_libraries(${SCRIPT_EXE}
    PRIVATE
        libraries
        scripts
        lib_sfml
)

target_sources(${SCRIPT_EXE}
    PRIVATE
        script.cpp
)

set(SFML_EXE sfml_exe)
add_executable(${SFML_EXE} WIN32)
target_link_libraries(${SFML_EXE}
    PRIVATE
        lib_sfml
)
target_sources(${SFML_EXE}
    PRIVATE
        sfml_window.cpp
)