cmake_minimum_required(VERSION 2.8.11)
project(relocate C)
set(VERSION "1.2.2")
set(CMAKE_C_STANDARD 99)
if(CMAKE_CL_64)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS /Wall")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")
endif()

enable_testing()
add_subdirectory(test)

configure_file(version.h.in version.h)

include_directories("${CMAKE_BINARY_DIR}")

add_library(relocate
	    STATIC reloc.c reloc_error.c)

add_executable(reloc main.c)

target_link_libraries(reloc relocate)

install(TARGETS reloc
	RUNTIME DESTINATION bin)
