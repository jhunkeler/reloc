cmake_minimum_required(VERSION 2.8.11)
project(reloc C)
set(VERSION "1.0.0")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")

configure_file(version.h.in version.h)
include_directories("${CMAKE_BINARY_DIR}")
add_executable(reloc main.c reloc.h)
install(TARGETS reloc
	RUNTIME DESTINATION bin)
