project(tests)

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_BINARY_DIR}")
add_executable(test_reloc_match test_reloc_match.c)
add_executable(test_reloc_read test_reloc_read.c)

target_link_libraries(test_reloc_match relocate)
target_link_libraries(test_reloc_read relocate)

add_test(test_reloc_match test_reloc_match)
add_test(test_reloc_read test_reloc_read)
