cmake_minimum_required(VERSION 2.7)
project(p2 C)

set(CMAKE_C_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(p2 main.c card_pile.c card_pile.h basic.h basic.c animation.h animation.c log.c log.h mgtk.h mgtk.c)
# Compile flags
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Werror -Wall -Wextra -Wconversion -Wvla -Wpedantic" CACHE STRING "Set C++ Compiler Flags" FORCE)


TARGET_LINK_LIBRARIES(p2 ${GTK3_LIBRARIES})