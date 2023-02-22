add_library(SYMBOLICORE_SRC OBJECT
    space.cpp
    #expression.cpp
)

if(COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()
