cmake_minimum_required (VERSION 3.12)

# Explicitly set the compiler search strategy before project()
set (CMAKE_CXX_COMPILER_INIT "")

# Preferred compilers in order
if (WIN32)
    # On Windows, prefer MSVC, then MinGW
    set (
        CMAKE_CXX_COMPILER_CANDIDATES
        "cl.exe" # MSVC
        "g++" # MinGW
        "clang++" # Clang
    )
elseif (UNIX)
    # On Unix-like systems, prefer gcc, then clang
    set (CMAKE_CXX_COMPILER_CANDIDATES "g++" # GCC
                                       "clang++" # Clang
    )
endif ()

# Try to find a suitable compiler
foreach (compiler ${CMAKE_CXX_COMPILER_CANDIDATES})
    find_program (DETECTED_CXX_COMPILER ${compiler})
    if (DETECTED_CXX_COMPILER)
        set (CMAKE_CXX_COMPILER ${DETECTED_CXX_COMPILER})
        break ()
    endif ()
endforeach ()

# Fallback error handling
if (NOT CMAKE_CXX_COMPILER)
    message (
        FATAL_ERROR "No C++ compiler found. Please install GCC, Clang, or MSVC."
    )
endif ()

# 并行编译选项
option (ENABLE_PARALLEL_BUILD "Enable parallel compilation" ON)

# 检测可用的CPU核心数
include (ProcessorCount)
ProcessorCount (CORE_COUNT)
if (CORE_COUNT EQUAL 0)
    set (CORE_COUNT 1)
endif ()

# 根据CPU核心数设置并行编译参数
if (ENABLE_PARALLEL_BUILD)
    # 对于不同的构建系统设置并行编译标志
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
        # Visual Studio 多进程编译
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif ()

    # Ninja生成器（跨平台）
    if (CMAKE_GENERATOR STREQUAL "Ninja")
        if (CORE_COUNT GREATER 1)
            set (NINJA_PARALLEL_JOBS "-j${CORE_COUNT}")
            set (CMAKE_BUILD_TYPE_INIT
                 "${CMAKE_BUILD_TYPE_INIT} ${NINJA_PARALLEL_JOBS}"
            )
        endif ()
    endif ()

    # Unix Makefiles 并行编译
    if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        set (CMAKE_MAKE_PROGRAM_ARGS "-j${CORE_COUNT}" CACHE STRING "")
    endif ()
endif ()

project (debug_macro VERSION 1.0.0 LANGUAGES CXX)

# Modern CMake practices
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# Enable testing early
enable_testing ()

# Include CTest module for additional testing capabilities
include (CTest)

# Use modern include what you use and export compile commands Optional IWYU
# support
option (ENABLE_IWYU "Enable Include What You Use" OFF)
if (ENABLE_IWYU)
    find_program (IWYU_PATH NAMES include-what-you-use iwyu)
    if (IWYU_PATH)
        set (CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
    else ()
        message (WARNING "IWYU requested but not found. Disabling.")
    endif ()
endif ()
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# check if it is the main project
string (COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
                IS_MAIN_PROJECT
)

# add options to control the compilation of tests and benchmarks
option (ENABLE_TESTS "Enable unit tests" ${IS_MAIN_PROJECT})
option (ENABLE_BENCHMARKS "Enable performance benchmarks" OFF)

# Create header-only library
add_library (${PROJECT_NAME} INTERFACE)
target_include_directories (
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)

# Add standard warning levels with cross-platform support
if (MSVC)
    set (WARNING_FLAGS /W4)
else ()
    set (WARNING_FLAGS -Wall -Wextra -Wpedantic)
endif ()

# Example executables
add_executable (debug_macro_basic_example examples/basic_usage.cpp)
target_link_libraries (debug_macro_basic_example ${PROJECT_NAME})
target_compile_options (debug_macro_basic_example PRIVATE ${WARNING_FLAGS})

add_executable (debug_macro_advanced_example examples/advanced_usage.cpp)
target_link_libraries (debug_macro_advanced_example ${PROJECT_NAME})
target_compile_options (debug_macro_advanced_example PRIVATE ${WARNING_FLAGS})

# Conditionally add tests and benchmarks
if (ENABLE_TESTS)
    enable_testing ()
    add_subdirectory (tests)
endif ()

if (ENABLE_BENCHMARKS)
    add_subdirectory (benchmarks)
endif ()

# Installation configuration
include (GNUInstallDirs)
install (
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install (DIRECTORY include/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Export CMake configuration files
install (EXPORT ${PROJECT_NAME}_Targets FILE ${PROJECT_NAME}Targets.cmake
         NAMESPACE ${PROJECT_NAME}::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Optional: Create and install CMake package config file
include (CMakePackageConfigHelpers)
write_basic_package_version_file (
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
