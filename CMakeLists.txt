cmake_minimum_required(VERSION 2.8.11)
project(relocate C)
set(VERSION "1.3.0")
set(CMAKE_C_STANDARD 99)
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"
   OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
	# C4996: The POSIX name for this item is deprecated
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS /W4 /wd4996")
endif()

enable_testing()
include(CTest)
add_subdirectory(test)

configure_file(version.h.in version.h)

include_directories("${CMAKE_BINARY_DIR}")

add_library(relocate
	    STATIC reloc.c reloc_error.c)

add_executable(reloc main.c)

target_link_libraries(reloc relocate)

install(TARGETS reloc
	RUNTIME DESTINATION bin)
