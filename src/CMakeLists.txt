add_library(SYMBOLICORE_SRC OBJECT
    logical.cpp
    integer.cpp
    real.cpp
    operators.cpp
    space.cpp
    expression.cpp
)

if(COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()
