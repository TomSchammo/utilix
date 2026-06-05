include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.15.0
    SYSTEM
)
FetchContent_MakeAvailable(Catch2)

# Catch2's own sources shouldn't run through our static analysers either.
foreach(_catch_target Catch2 Catch2WithMain)
    set_target_properties(${_catch_target} PROPERTIES
        CXX_CLANG_TIDY "" CXX_CPPCHECK "")
endforeach()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

add_executable(utilix_tests
    test_types.cpp
    test_memory.cpp
)
target_link_libraries(utilix_tests PRIVATE
    utilix::utilix
    Catch2::Catch2WithMain
    utilix_sanitizers)

catch_discover_tests(utilix_tests)
