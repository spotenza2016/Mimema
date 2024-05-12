cmake_minimum_required(VERSION 3.17)
project(Mimema)

# For standalone exe
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(CMAKE_CXX_STANDARD 14)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(D:\\Downloads\\glfw-3.3.9\\glfw-3.3.9 glfw)

find_package(OpenGL REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "D:\\Downloads\\glew-2.1.0-win32\\glew-2.1.0")
find_package(GLEW REQUIRED)

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/0.9.9.8
)

FetchContent_MakeAvailable(glm)

include_directories("D:\\Downloads\\STB\\stb")

# Add WIN32 after exe name to avoid command prompt
add_executable(Mimema WIN32 main.cpp OpenGlHandler.cpp OpenGlHandler.h EngineCore.cpp EngineCore.h Model.h Model.cpp Object.cpp Object.h Level.cpp Level.h Camera.cpp Camera.h Material.cpp Material.h EngineState.cpp EngineState.h)
target_link_libraries(Mimema glfw GLEW::GLEW OpenGL32 glm)