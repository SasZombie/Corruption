cmake_minimum_required(VERSION 3.10)

project(Corruption)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_WARNINGS "-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Target platform: Windows")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNINGS}")
    set(PLATFORM_LIBS raylib)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Target platform: Linux")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNINGS} -fsanitize=address,undefined,leak")
    set(PLATFORM_LIBS raylib m pthread X11 GL dl)

else()
    message(FATAL_ERROR "Unsupported or unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

add_executable(main main.cpp colorManipulation.cpp)
target_link_libraries(main ${PLATFORM_LIBS})
