# -----------
#   Define
# -----------
cmake_minimum_required(VERSION 3.28)
project(gdumper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------
#   Core
# -----------
add_library(mylib
    src/argsmanager.cpp
    src/guimanager.cpp
    src/processmanager.cpp
    src/pidutils.cpp
)

target_include_directories(mylib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(gdumper src/main.cpp)
target_link_libraries(gdumper PRIVATE mylib)

# -----------
#   Curses
# -----------
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
message(STATUS "Curses enabled")

# -----------
#   Test
# -----------
option(ENABLE_TESTS "Enable building tests" OFF)
if(ENABLE_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(tests 
        test/pidutilstest.cpp
        test/argsmanagertest.cpp
        test/processmanagertest.cpp
    )

    target_link_libraries(tests PRIVATE mylib GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(tests)
    message(STATUS "Gtest enabled")
endif()


message(STATUS "Done!")
