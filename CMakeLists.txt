cmake_minimum_required (VERSION 3.12)

if (NOT CMAKE_C_COMPILER)
    find_program (CMAKE_C_COMPILER NAMES gcc clang cl)
endif ()

if (NOT CMAKE_CXX_COMPILER)
    find_program (CMAKE_CXX_COMPILER NAMES g++ clang++ cl)
endif ()

project (debug_macro VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# 检查是否为主项目
string (COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
                IS_MAIN_PROJECT
)

# 添加选项控制测试和基准测试的编译
option (ENABLE_TESTS "Enable unit tests" ${IS_MAIN_PROJECT})
option (ENABLE_BENCHMARKS "Enable performance benchmarks" OFF)

# 创建库 (头文件库)
add_library (${PROJECT_NAME} INTERFACE)
target_include_directories (
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)

# 测试配置
if (ENABLE_TESTS)
    enable_testing ()
    add_subdirectory (tests)
endif ()

# 基准测试配置
if (ENABLE_BENCHMARKS)
    add_subdirectory (benchmarks)
endif ()

# 示例程序
add_executable (debug_macro_basic_example examples/basic_usage.cpp)
target_link_libraries (debug_macro_basic_example ${PROJECT_NAME})

add_executable (debug_macro_advanced_example examples/advanced_usage.cpp)
target_link_libraries (debug_macro_advanced_example ${PROJECT_NAME})

# 编译器警告选项
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

# 安装配置
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

# 导出 CMake 配置文件
install (EXPORT ${PROJECT_NAME}_Targets FILE ${PROJECT_NAME}Targets.cmake
         NAMESPACE ${PROJECT_NAME}::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
