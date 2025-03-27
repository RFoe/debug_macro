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

project (debug_macro VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# 检测CPU核心数并设置并行编译
include (ProcessorCount)
ProcessorCount (CPU_CORES)

# 如果检测到CPU核心数，则设置并行编译
if (CPU_CORES GREATER 0)
    # 针对不同编译器的并行编译设置
    if (MSVC)
        # Visual Studio编译器
        add_compile_options ("/MP${CPU_CORES}")
    elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        # GCC/G++编译器
        add_compile_options ("-j${CPU_CORES}")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang编译器
        add_compile_options ("-j${CPU_CORES}")
    endif ()

    # 设置并行构建选项
    set (CMAKE_BUILD_TYPE_PARALLEL ON)
    set (CMAKE_PARALLEL_COMPILE_JOBS ${CPU_CORES})
endif ()

# check if it is the main project
string (COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
                IS_MAIN_PROJECT
)

# add options to control the compilation of tests and benchmarks
option (ENABLE_TESTS "Enable unit tests" ${IS_MAIN_PROJECT})
option (ENABLE_BENCHMARKS "Enable performance benchmarks" ON)

# create a library header file
add_library (${PROJECT_NAME} INTERFACE)
target_include_directories (
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)

# test configuration
if (ENABLE_TESTS)
    enable_testing ()
    add_subdirectory (tests)
endif ()

# benchmark test configuration
if (ENABLE_BENCHMARKS)
    enable_testing ()
    add_subdirectory (benchmarks)
endif ()

# example program
add_executable (debug_macro_basic_example examples/basic_usage.cpp)
target_link_libraries (debug_macro_basic_example ${PROJECT_NAME})

add_executable (debug_macro_advanced_example examples/advanced_usage.cpp)
target_link_libraries (debug_macro_advanced_example ${PROJECT_NAME})

# compiler warning options
if (MSVC)
    target_compile_options (debug_macro_basic_example PRIVATE /W4)
    target_compile_options (debug_macro_advanced_example PRIVATE /W4)
else ()
    target_compile_options (
        debug_macro_basic_example PRIVATE -Wall -Wextra -pedantic
    )
    target_compile_options (
        debug_macro_advanced_example PRIVATE -Wall -Wextra -pedantic
    )
endif ()

# installation configuration
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

# export the CMake configuration file
install (EXPORT ${PROJECT_NAME}_Targets FILE ${PROJECT_NAME}Targets.cmake
         NAMESPACE ${PROJECT_NAME}::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
