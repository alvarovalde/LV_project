cmake_minimum_required(VERSION 3.28)
project(gravity_simulator)

set(CMAKE_CXX_STANDARD 17)

# ==========================
#  1) Fetch GLM via CMake
# ==========================
include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
)
FetchContent_MakeAvailable(glm)

# ==========================
#  2) Define GLFW as IMPORTED
# ==========================
# You have libglfw3.a in lib/lib-mingw-w64/, plus headers in lib/include.
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/lib-mingw-w64/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include"
)

# ==========================
#  3) Define GLEW as IMPORTED
# ==========================
# We'll assume you want STATIC GLEW. Typically that file is called 'glew32s.lib',
# but if you only have 'glew32.lib' and you define GLEW_STATIC, it means you have
# a static library renamed or a custom build. Adjust the filename as needed.
add_library(glew STATIC IMPORTED)
set_target_properties(glew PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/glew32.lib"
        INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew"
)

# If you want a static build, you usually define GLEW_STATIC:
# (This ensures #ifdef GLEW_STATIC in the GLEW headers is recognized.)
add_compile_definitions(GLEW_STATIC)

# ==========================
#  4) Create the Executable
# ==========================
add_executable(gravity_simulator
        main.cpp
        Object.h
        Object.cpp
        ISA_atmosphere.h
        ISA_atmosphere.cpp
)

# ==========================
#  5) Link Everything
# ==========================
# - Link to glfw (which also sets up header paths),
# - Link to system OpenGL on Windows (opengl32),
# - Link to glm via its CMake target,
# - Link to our glew imported target.
target_link_libraries(gravity_simulator
        PRIVATE
        glfw
        opengl32
        glm::glm
        glew
)
