cmake_minimum_required(VERSION 2.8.11)
project(reloc C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")

add_executable(reloc main.c reloc.h)
