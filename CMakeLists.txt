cmake_minimum_required(VERSION 3.28)
project(gdumper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Loading target sources...")
add_executable(gdumper
    src/argsmanager.cpp
    src/guimanager.cpp
    src/main.cpp
    src/processmanager.cpp
    src/pidutils.cpp
)

target_include_directories(gdumper PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

message(STATUS "Loading curses...")
find_package(Curses REQUIRED)
if (CURSES_FOUND)
    target_include_directories(gdumper PRIVATE ${CURSES_INCLUDE_DIR})
    target_link_libraries(gdumper PRIVATE ${CURSES_LIBRARIES})
else()
    message(FATAL_ERROR "Curses not found")
endif()

find_library(TINFO_LIB tinfo)
if(TINFO_LIB)
    target_link_libraries(gdumper PRIVATE ${TINFO_LIB})
endif()

message(STATUS "Done!")
