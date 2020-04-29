cmake_minimum_required(VERSION 3.15)
project(DnkvwRaylibExample)

find_package(raylib 2.5.0 REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(DnkvwRaylibExample main.c)

target_link_libraries(DnkvwRaylibExample raylib)

