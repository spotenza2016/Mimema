cmake_minimum_required(VERSION 3.17)
project(Mimema)

# For standalone exe
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)

# GlFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# GLEW
find_package(OpenGL REQUIRED)
FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
)
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
    FetchContent_Populate(glew)

    add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy
        GIT_TAG 075395620a504c0cdcaf9bab3d196db16a043de7
)
FetchContent_GetProperties(tracy)
if(NOT tracy_POPULATED)
    FetchContent_Populate(tracy)

    add_subdirectory(${tracy_SOURCE_DIR} ${tracy_BINARY_DIR})
endif()


# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/0.9.9.8
)
FetchContent_MakeAvailable(glm)

# STB_IMAGE
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/gracicot/stb-cmake.git
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
    FetchContent_Populate(stb)

    add_subdirectory(${stb_SOURCE_DIR} ${stb_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Add WIN32 after exe name to avoid command prompt (will disable cout)
add_executable(Mimema main.cpp Core.cpp Core.h Object.cpp Object.h Level.cpp Level.h Camera.cpp Camera.h Material.cpp Material.h RenderState.cpp RenderState.h Octree.cpp Octree.h Collider.cpp Collider.h Renderable.cpp Renderable.h Shader.cpp Shader.h PointLight.cpp PointLight.h DirectionalLight.cpp DirectionalLight.h HMI.cpp HMI.h)
target_link_libraries(Mimema glfw libglew_static OpenGL32 glm stb::image TracyClient)