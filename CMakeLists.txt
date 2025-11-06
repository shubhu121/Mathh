cmake_minimum_required(VERSION 3.16)
project(MathEngineUTF8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/engine/parser.cpp
    src/engine/differentiator.cpp
    src/engine/integrator.cpp
    src/engine/simplifier.cpp
    src/engine/limit_calculator.cpp
    src/engine/matrix_operations.cpp
    src/engine/latex_exporter.cpp
    src/ui/renderer.cpp
    src/ui/text_renderer.cpp
    src/ui/plotter.cpp
)

# Executable
add_executable(MathEngineUTF8 ${SOURCES})

# Link libraries
target_link_libraries(MathEngineUTF8
    ${SDL2_LIBRARIES}
    SDL2_ttf
    OpenGL::GL
    GLEW::GLEW
)

# Copy assets to build directory
add_custom_command(TARGET MathEngineUTF8 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

# Windows specific settings
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif()
