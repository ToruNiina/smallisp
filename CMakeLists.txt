cmake_minimum_required(VERSION 3.0)
project(smallisp)

add_executable(smallisp "${PROJECT_SOURCE_DIR}/src/main.cpp")
set_target_properties(smallisp
    PROPERTIES
    COMPILE_FLAGS "-std=c++17 -O2 -Wall -Wextra -Wpedantic"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
