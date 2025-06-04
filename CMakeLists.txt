cmake_minimum_required(VERSION 3.28)
project(dumper)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(dumper ${SOURCES})
target_include_directories(dumper PRIVATE include)

find_package(Curses REQUIRED)
target_link_libraries(dumper PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(dumper PRIVATE ncurses tinfo)
