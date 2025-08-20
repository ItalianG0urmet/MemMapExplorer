cmake_minimum_required(VERSION 3.28)
project(dumper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(dumper ${SOURCES})
target_include_directories(dumper PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

message(STATUS "Loading target sources...")
target_sources(dumper PRIVATE
    ${CMAKE_SOURCE_DIR}/src/argsmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/guimanager.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/processmanager.cpp
)

message(STATUS "Loading curses...")
find_package(Curses REQUIRED)
target_link_libraries(dumper PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(dumper PRIVATE ncurses tinfo)
message(STATUS "Done!")
