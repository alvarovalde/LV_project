cmake_minimum_required(VERSION 3.28)
project(gravity_simulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(gravity_simulator main.cpp
        Object.h
        Object.cpp)


include_directories(lib)

add_library(glfw STATIC IMPORTED)


set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/lib-mingw-w64/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/include")
target_link_libraries(gravity_simulator glfw opengl32)