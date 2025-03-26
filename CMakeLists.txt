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

# check if it is the main project
string (COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
                IS_MAIN_PROJECT
)

# add options to control the compilation of tests and benchmarks
option (ENABLE_TESTS "Enable unit tests" ${IS_MAIN_PROJECT})
option (ENABLE_BENCHMARKS "Enable performance benchmarks" OFF)

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
