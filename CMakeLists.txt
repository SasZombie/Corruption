cmake_minimum_required(VERSION 3.10)

project(Corruption)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi -fsanitize=undefined -fsanitize=address -fsanitize=leak")

add_executable(main main.cpp colorManipulation.cpp)


target_link_libraries(main raylib m pthread X11 GL dl)