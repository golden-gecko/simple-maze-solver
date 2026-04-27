cmake_minimum_required(VERSION 4.1.2)

project(simple-maze-solver)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_executable(
    simple-maze-solver
    AStar.cpp AStar.hpp
    Main.cpp
    Maze.cpp Maze.hpp
    Node.cpp Node.hpp
    Renderer.cpp Renderer.hpp
    Search.cpp Search.hpp
)

target_link_libraries(simple-maze-solver PRIVATE glfw opengl32)

set_target_properties(simple-maze-solver PROPERTIES
    CXX_EXTENSIONS NO
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
