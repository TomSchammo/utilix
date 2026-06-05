cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(utilix VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# --- ccache (if available) -------------------------------------------------
find_program(UTILIX_CCACHE ccache)
if(UTILIX_CCACHE AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${UTILIX_CCACHE}")
endif()

# --- Library target --------------------------------------------------------
add_library(utilix INTERFACE)
add_library(utilix::utilix ALIAS utilix)

target_compile_features(utilix INTERFACE cxx_std_20)

target_sources(utilix INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
        include/utilix/types.hpp
        include/utilix/math.hpp
        include/utilix/memory.hpp
        include/utilix/timing.hpp
)

# --- Developer-only knobs (top-level builds only) --------------------------
# These never leak into a parent project that consumes utilix via FetchContent.
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Header self-containment check: CMake generates a
    # `utilix_verify_interface_header_sets` target that compiles each header
    # in isolation. This is what makes the warnings and analysers below
    # actually have something to chew on.
    set_target_properties(utilix PROPERTIES VERIFY_INTERFACE_HEADER_SETS TRUE)

    # --- Warnings -----------------------------------------------------------
    # Attached to utilix's INTERFACE so they propagate to anything that
    # consumes the library (tests, fuzzers, the auto-generated header-verify
    # target) but never reach FetchContent dependencies like Catch2. Gated on
    # PROJECT_IS_TOP_LEVEL above ensures they don't leak to downstream
    # consumers either.
    target_compile_options(utilix INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Wshadow>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wconversion>
        $<$<CXX_COMPILER_ID:GNU>:-Wsign-conversion>
    )

    # --- LTO for optimised builds ------------------------------------------
    include(CheckIPOSupported)
    check_ipo_supported(RESULT _ipo_supported OUTPUT _ipo_output)
    if(_ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE        TRUE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
    else()
        message(WARNING "LTO not supported: ${_ipo_output}")
    endif()

    # --- Native arch for optimised builds ----------------------------------
    # ARM toolchains take -mcpu=native; x86 takes -march=native. Mixing them
    # up silently produces a non-native build, so branch on the processor.
    if(NOT CMAKE_CROSSCOMPILING AND NOT MSVC)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|arm)")
            set(_utilix_native_arch_flag "-mcpu=native")
        else()
            set(_utilix_native_arch_flag "-march=native")
        endif()
        foreach(_config RELEASE RELWITHDEBINFO)
            string(APPEND CMAKE_C_FLAGS_${_config}   " ${_utilix_native_arch_flag}")
            string(APPEND CMAKE_CXX_FLAGS_${_config} " ${_utilix_native_arch_flag}")
        endforeach()
    endif()

    # --- ASan + UBSan as an opt-in INTERFACE target ------------------------
    # Sanitizers are an instrumentation choice for specific binaries (our
    # tests), not a build-wide setting: instrumenting Catch2 wastes cycles
    # and risks ODR weirdness when mixing instrumented/uninstrumented code in
    # the same binary. Targets that want sanitizers link `utilix_sanitizers`
    # explicitly. Feature-detected because some toolchains (Homebrew GCC on
    # macOS) accept the compile flag but can't link against libasan.
    add_library(utilix_sanitizers INTERFACE)
    if(NOT MSVC)
        include(CheckLinkerFlag)
        check_linker_flag(CXX "-fsanitize=address,undefined"
                          UTILIX_LINKER_HAS_SANITIZERS)
        if(UTILIX_LINKER_HAS_SANITIZERS)
            target_compile_options(utilix_sanitizers INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined;-fno-omit-frame-pointer>)
            target_link_options(utilix_sanitizers INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
        else()
            message(STATUS "sanitizers: linker rejected -fsanitize=address,undefined; skipping")
        endif()
    endif()

    # --- Future-use flags (commented out, kept here as a recipe) -----------
    # These are intentionally disabled for utilix. Uncomment in projects
    # where they fit. Each block notes the trap that makes it non-trivial.
    #
    # -- Per-binary machine properties (spill onto deps in the build tree) --
    # Hardening: low-cost, deps benefit too. Some distros enable these by
    # default; check before stacking.
    # foreach(_config RELEASE RELWITHDEBINFO DEBUG)
    #     string(APPEND CMAKE_C_FLAGS_${_config}
    #         " -fstack-protector-strong -D_FORTIFY_SOURCE=2")
    #     string(APPEND CMAKE_CXX_FLAGS_${_config}
    #         " -fstack-protector-strong -D_FORTIFY_SOURCE=2")
    # endforeach()
    #
    # Hidden symbol visibility: smaller binaries, cleaner exported ABI. Fine
    # for header-only/static libs; for shared libs you must annotate the
    # public API with visibility("default") or nothing will be exported.
    # set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    # set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
    #
    # -- Whole-program ABI commitments (set globally, configure deps too) ---
    # -fno-exceptions: bans throw/try/catch in every TU. Deps that use
    # exceptions either need their own no-exceptions mode set via macros
    # (e.g. FMT_EXCEPTIONS=0) or they fail to compile -- which is the
    # enforcement, not a regression. Mix-and-match leads to terminate() at
    # runtime, never do that.
    # foreach(_lang C CXX)
    #     string(APPEND CMAKE_${_lang}_FLAGS " -fno-exceptions")
    # endforeach()
    #
    # -fno-rtti: same shape as -fno-exceptions. Breaks dynamic_cast and
    # typeid; deps relying on them won't compile. Whole-program decision.
    # string(APPEND CMAKE_CXX_FLAGS " -fno-rtti")
    #
    # -ffast-math: tempting performance flag, behaves like a whole-program
    # commitment because of crtfastmath: on glibc, linking ANY TU built with
    # -ffast-math pulls in a static ctor that flips the FPU into FTZ/DAZ
    # mode process-wide before main(). That contaminates every other TU,
    # including deps that don't opt in. Also changes <cmath> semantics
    # (e.g. std::isnan may fold to false). Only enable if the entire binary
    # is OK with relaxed FP semantics.
    # foreach(_config RELEASE RELWITHDEBINFO)
    #     string(APPEND CMAKE_C_FLAGS_${_config}   " -ffast-math")
    #     string(APPEND CMAKE_CXX_FLAGS_${_config} " -ffast-math")
    # endforeach()
    #
    # -- Per-target dev flags (link into specific binaries, like sanitizers)
    # Coverage: only meaningful on code under test. Define as INTERFACE and
    # link to test/fuzz targets. Pair with --coverage on the linker too.
    # add_library(utilix_coverage INTERFACE)
    # target_compile_options(utilix_coverage INTERFACE
    #     $<$<CONFIG:Debug>:--coverage>)
    # target_link_options(utilix_coverage INTERFACE
    #     $<$<CONFIG:Debug>:--coverage>)
    #
    # -Werror: per-project taste call. Add to the warnings INTERFACE above:
    # target_compile_options(utilix INTERFACE
    #     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
    #     $<$<CXX_COMPILER_ID:MSVC>:/WX>)

    # --- clang-tidy (auto-detected) ----------------------------------------
    find_program(UTILIX_CLANG_TIDY clang-tidy)
    if(UTILIX_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY "${UTILIX_CLANG_TIDY}")
        message(STATUS "clang-tidy: ${UTILIX_CLANG_TIDY}")
    else()
        message(STATUS "clang-tidy: not found (skipping)")
    endif()

    # --- cppcheck (auto-detected) ------------------------------------------
    find_program(UTILIX_CPPCHECK cppcheck)
    if(UTILIX_CPPCHECK)
        # FetchContent puts third-party sources under build/_deps; we don't
        # want their findings to fail our build.
        set(CMAKE_CXX_CPPCHECK
            "${UTILIX_CPPCHECK}"
            "--enable=warning,style,performance,portability"
            "--inline-suppr"
            "--suppress=missingIncludeSystem"
            "--suppress=unusedStructMember"
            "--suppress=*:${CMAKE_BINARY_DIR}/_deps/*"
            # Test assertions on constexpr functions are tautological by
            # design; this check just fires on every STATIC_REQUIRE.
            "--suppress=knownConditionTrueFalse:${CMAKE_CURRENT_SOURCE_DIR}/tests/*"
            "--error-exitcode=1"
            "--std=c++20"
            "--language=c++")
        message(STATUS "cppcheck: ${UTILIX_CPPCHECK}")
    else()
        message(STATUS "cppcheck: not found (skipping)")
    endif()

    # --- Tests --------------------------------------------------------------
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

# --- Install / package config ---------------------------------------------
install(TARGETS utilix
        EXPORT utilixTargets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT utilixTargets
        FILE utilixTargets.cmake
        NAMESPACE utilix::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utilix)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/utilixConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utilix)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/utilixConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utilix)
