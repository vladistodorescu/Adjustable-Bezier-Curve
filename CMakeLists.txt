cmake_minimum_required(VERSION 3.27)
project(Assignment_5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# Tell CMake where GLFW is
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(Assignment_5 main.cpp)

target_link_libraries(Assignment_5 PRIVATE
        OpenGL::GL
        glfw
)

# Optional: silence OpenGL deprecation warnings on macOS
target_compile_definitions(Assignment_5 PRIVATE GL_SILENCE_DEPRECATION)
