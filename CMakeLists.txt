cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(utilix VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional clang-tidy integration
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
if(ENABLE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,clang-analyzer-*,boost-*,cppcoreguidelines-*,bugprone-*,modernize-*,-modernize-pass-by-value,-modernize-use-trailing-return-type,-bugprone-easily-swappable-parameters,-bugprone-suspicious-include,-readability-identifier-length)
endif()

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

if (MSVC)
    # additional warning levels for msvc (commonly used on windows)
    add_compile_options(/W4)
else()
    # additional warnings for other compilers
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)
endif()

# Add sign conversion warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wconversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wsign-conversion)
endif()

# Collect header files
file(GLOB_RECURSE MY_LIBRARY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# Create an INTERFACE library (header-only)
add_library(utilix INTERFACE)
target_include_directories(utilix INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/utilix>
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/utilix)
