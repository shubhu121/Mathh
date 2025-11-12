cmake_minimum_required(VERSION 3.16)
project(Mathh)

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
    src/engine/partial_derivative.cpp
    src/engine/multivariate_integrator.cpp
    src/engine/implicit_differentiation.cpp
    src/engine/linear_transformation.cpp
    src/engine/taylor_series.cpp
    src/engine/parametric_curve.cpp
    src/engine/laplace_transform.cpp
    src/engine/fourier_series.cpp
    src/engine/differential_equations.cpp
    src/engine/vector_calculus.cpp
    src/engine/complex_numbers.cpp
    src/engine/sequences_series.cpp
    src/engine/numerical_methods.cpp
    src/engine/eigenvalues.cpp
    src/engine/statistics.cpp
    src/engine/polynomial_operations.cpp
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
