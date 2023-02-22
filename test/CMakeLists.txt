set(UNIT_TESTS
    test_expression
)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} symbolicore)
    add_test(${TEST} ${TEST})
endforeach()
