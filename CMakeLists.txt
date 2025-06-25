cmake_minimum_required(VERSION 3.10)

project(Corruption)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_WARNINGS "-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic")

option(STATIC_BUILD "Link statically, embed shaders, and remove runtime dependencies" OFF)

add_executable(main main.cpp colorManipulation.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Target platform: Windows")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNINGS}")

    if(STATIC_BUILD)
        message(STATUS "Static build enabled")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,-Bstatic")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,-Bstatic -Wl,-Bsymbolic")

        set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

        target_compile_definitions(main PRIVATE bakedShaders)
    endif()

    set(PLATFORM_LIBS raylib winmm gdi32)

#I am lazy to do this for linux
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Target platform: Linux")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNINGS}")

    if(STATIC_BUILD)
        message(STATUS "Static build enabled")

        # Static linking for GCC on Linux
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

        # Optionally prefer static libraries
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

        # Embed shaders
        target_compile_definitions(main PRIVATE bakedShaders)

        # Set libraries needed for raylib (statically)
        set(PLATFORM_LIBS raylib m pthread X11 GL dl rt)
    else()
        # With sanitizers, only dynamic linking works well
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak")
        set(PLATFORM_LIBS raylib m pthread X11 GL dl)
    endif()

else()
    message(FATAL_ERROR "Unsupported or unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

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
