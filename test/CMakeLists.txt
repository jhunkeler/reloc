project(tests)

set(INPUT_SEARCH
	/_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_)
set(INPUT_REPLACE
	_ctest_replaced_this)
set(INPLACE_FN
	test_data2.bin)
set(INPUT_FN
	test_data.bin)
set(OUTPUT_FN
	test_data.out)

configure_file(${INPUT_FN} ${INPUT_FN} COPYONLY)
configure_file(${INPUT_FN} ${INPLACE_FN} COPYONLY)
file(REMOVE ${INPLACE_FN})
file(REMOVE ${OUTPUT_FN})

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_BINARY_DIR}")

function(add_reloc_test target)
	add_executable(${target} ${ARGN})
	target_link_libraries(${target} relocate)
	add_test(${target} ${target})
endfunction()

function(add_exec_test target)
	add_test(NAME ${target} COMMAND ${ARGN})
endfunction()

add_reloc_test(test_reloc_match test_reloc_match.c)
add_reloc_test(test_reloc_read test_reloc_read.c)
add_reloc_test(test_reloc_write test_reloc_write.c)
add_exec_test(test_exec_success reloc "${INPUT_SEARCH}" "${INPUT_REPLACE}" "${INPUT_FN}" "${OUTPUT_FN}")
add_exec_test(test_exec_input_file_only_success reloc "${INPUT_SEARCH}" "${INPUT_REPLACE}" "${INPLACE_FN}")

# Windows users need grep too. "findstr" and "find" were not reliable here
add_exec_test(test_exec_result grep "${INPUT_REPLACE}" "${OUTPUT_FN}")
add_exec_test(test_exec_input_file_only_result grep "${INPUT_REPLACE}" "${INPLACE_FN}")
