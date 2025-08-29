cmake_minimum_required(VERSION 3.28)
project(gdumper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(gdumper ${SOURCES})
target_include_directories(gdumper PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/gdumper
)

message(STATUS "Loading target sources...")
target_sources(gdumper PRIVATE
    ${CMAKE_SOURCE_DIR}/src/argsmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/guimanager.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/processmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/pidutils.cpp
)


message(STATUS "Loading curses...")
find_package(Curses REQUIRED)
target_link_libraries(gdumper PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(gdumper PRIVATE ncurses tinfo)
message(STATUS "Done!")
