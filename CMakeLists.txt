cmake_minimum_required(VERSION 3.8)
project(gravity_simulator)

set(CMAKE_CXX_STANDARD 17)

# ==========================
#  1) Fetch GLM via CMake
# ==========================
# This is the newer CMake “FetchContent” mechanism,
# which automatically downloads (clones) the GLM library
# from GitHub at the specified commit (GIT_TAG).
# It then configures it so you can link to glm::glm
# in your code.
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
add_library(glfw STATIC IMPORTED) # Declares a dummy CMake target called glfw which is “IMPORTED.” That means we are telling CMake where to find the library binary and headers, rather than letting CMake build the library itself.
# “STATIC” here means we’re treating this as a static library.
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/lib-mingw-w64/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include"
)
# set_target_properties(glfw PROPERTIES ...)
#IMPORTED_LOCATION – path to the actual .a or .lib file for GLFW.
#INTERFACE_INCLUDE_DIRECTORIES – the include path for the GLFW headers.
#By setting INTERFACE_INCLUDE_DIRECTORIES, any target that links to “glfw” automatically inherits -I<that-folder> on the compiler command line. So you won’t need a separate #include <...> path for your code – it’s built into the usage requirements of the “glfw” target.







# ==========================
#  3) Define GLEW as IMPORTED
# ==========================
add_library(glew STATIC IMPORTED
        include/constants.h)
set_target_properties(glew PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/glew32.lib"
        INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew"
)

# This adds a -DGLEW_STATIC compiler flag to all source files. Typically GLEW’s header uses #ifdef GLEW_STATIC to do some static-library-specific code paths.
add_compile_definitions(GLEW_STATIC)





# ==========================
#  4) Create the Executable
# ==========================
add_executable(gravity_simulator
        src/main.cpp
        src/Object.cpp
        src/atmospheric_models/ISA_atmosphere.cpp
        src/atmospheric_models/AtmosphereFactory.cpp
)


# 5) Tell the compiler to look in our own include/ folder
target_include_directories(gravity_simulator
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"   # so #include "Object.h" etc. works
)

# ==========================
#  5) Link Everything
# ==========================
# - Link to glfw (which also sets up header paths),
# - Link to system OpenGL on Windows (opengl32),
# - Link to glm via its CMake target,
# - Link to our glew imported target.
target_link_libraries(gravity_simulator
        #PRIVATE
        glfw
        opengl32
        glm::glm
        glew
)

# target_link_libraries(gravity_simulator PRIVATE ...)
#
#This tells CMake: “The executable gravity_simulator depends on glfw, opengl32, glm::glm, and glew.”
#
#Because you set INTERFACE_INCLUDE_DIRECTORIES for glfw and glew, your compiler automatically sees those header paths.
#
#opengl32 is the standard Windows OpenGL library.
#
#glm::glm is the target that FetchContent created for GLM.
#
#The PRIVATE keyword means these dependencies are only used internally by gravity_simulator. Another target depending on gravity_simulator would not automatically inherit those link/include settings.
