set(UNIT_TESTS
    test_paradigm
    test_logical
    test_integer
    test_real
    test_space
    test_expression
)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} symbolicore)
    add_test(${TEST} ${TEST})
endforeach()

add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS})
