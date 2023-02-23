add_library(SYMBOLICORE_SRC OBJECT
    space.cpp
    #logical.cpp
    #operators.cpp
    #expression.cpp
)

if(COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()
