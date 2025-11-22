cmake_minimum_required(VERSION 4.2)

project(simple-maze-solver)

add_executable(simple-maze-solver Main.cpp)

set_target_properties(simple-maze-solver PROPERTIES
    CXX_EXTENSIONS NO
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
