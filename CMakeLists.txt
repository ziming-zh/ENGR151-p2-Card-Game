cmake_minimum_required(VERSION 3.20)
project(P2_1)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(P2_1 main.c card_pile.c card_pile.h basic.h basic.c animation.h animation.c log.c log.h mgtk.h mgtk.c)
TARGET_LINK_LIBRARIES(P2_1 ${GTK3_LIBRARIES})