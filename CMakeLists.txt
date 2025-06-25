cmake_minimum_required(VERSION 3.10)

project(Corruption)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_WARNINGS "-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic")


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Target platform: Windows")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNINGS}")
    set(PLATFORM_LIBS raylib winmm gdi32)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Target platform: Linux")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNINGS} -fsanitize=address,undefined,leak")
    set(PLATFORM_LIBS raylib m pthread X11 GL dl)

else()
    message(FATAL_ERROR "Unsupported or unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()



add_executable(main main.cpp colorManipulation.cpp)
target_link_libraries(main ${PLATFORM_LIBS})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(main PROPERTIES OUTPUT_NAME "main")
else()
    set_target_properties(main PROPERTIES OUTPUT_NAME "main.out")
endif()

option(USE_BAKED_SHADERS "Use embedded shaders instead of external files" ON)

if(USE_BAKED_SHADERS)
    target_compile_definitions(main PRIVATE bakedShaders)
endif()
