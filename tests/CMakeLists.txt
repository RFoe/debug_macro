# Modern CMake practices
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# Fetch and configure Google Test
include (FetchContent)
FetchContent_Declare (
    googletest GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0 # Use the latest stable version
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)

# Add test executable
add_executable (debug_macro_tests advanced.cpp basic.cpp)

# Link against the project library and Google Test
target_link_libraries (debug_macro_tests PRIVATE debug_macro GTest::gtest_main)

# Enable testing
include (GoogleTest)
gtest_discover_tests (debug_macro_tests)

# Add benchmarks as CTest tests
add_test (NAME debug_macro_tests_ COMMAND debug_macro_tests
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Set properties for the benchmark test
set_tests_properties (
    debug_macro_tests_
    PROPERTIES LABELS "benchmark" TIMEOUT 300 # 5-minute timeout for benchmarks
)

# Optional: Add a custom target to run benchmarks
add_custom_target (
    run_tests COMMAND ${CMAKE_CTEST_COMMAND} -L "tests"
    COMMENT "Running performance benchmarks"
)
