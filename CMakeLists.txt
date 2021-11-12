cmake_minimum_required(VERSION 2.7)
project(p2 C)

set(CMAKE_C_STANDARD 11)


add_executable(p2 main.c card_pile.c card_pile.h basic.h basic.c animation.h animation.c log.c log.h)

# Compile flags
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Werror -Wall -Wextra -Wconversion -Wvla -Wpedantic" CACHE STRING "Set C++ Compiler Flags" FORCE)

