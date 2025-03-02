cmake_minimum_required(VERSION 3.28)
project(dumper)

set(CMAKE_CXX_STANDARD 20)

add_executable(dumper src/main.cpp
        src/manager/GuiManager.cpp
        include/GuiManager.h
        include/globals.h
        src/manager/ProcessManager.cpp
        include/ProcessManager.h)

find_package(Curses REQUIRED)
target_link_libraries(dumper PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(dumper PRIVATE ncurses tinfo)
