cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(utilix VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(utilix INTERFACE)
add_library(utilix::utilix ALIAS utilix)

target_compile_features(utilix INTERFACE cxx_std_20)

target_include_directories(utilix INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Developer-only knobs are gated on PROJECT_IS_TOP_LEVEL so they do not leak
# into a parent project that consumes utilix via FetchContent.
if(PROJECT_IS_TOP_LEVEL)
    option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
    if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,clang-analyzer-*,boost-*,cppcoreguidelines-*,bugprone-*,modernize-*,-modernize-pass-by-value,-modernize-use-trailing-return-type,-bugprone-easily-swappable-parameters,-bugprone-suspicious-include,-readability-identifier-length)
    endif()

    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wconversion)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wsign-conversion)
    endif()
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS utilix EXPORT utilixTargets)

install(EXPORT utilixTargets
        FILE utilixTargets.cmake
        NAMESPACE utilix::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utilix)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/utilixConfig.cmake"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/utilixTargets.cmake\")\n")

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utilix)
