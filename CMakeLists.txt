cmake_minimum_required(VERSION 3.15)

project(mandelbrot)

# Look for Raylib and Eigen
find_package(raylib REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(mandelbrot)

#add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(mandelbrot PRIVATE raylib Eigen3::Eigen $<$<PLATFORM_ID:Windows>:winmm>)

set_target_properties(mandelbrot PROPERTIES CXX_STANDARD 20)

target_compile_options(mandelbrot PRIVATE -march=native -Werror -Wall -Wextra -Wpedantic)

