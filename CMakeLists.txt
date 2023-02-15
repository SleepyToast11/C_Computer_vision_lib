cmake_minimum_required(VERSION 3.24)
project(funTimes C)

set(CMAKE_C_STANDARD 17)
add_compile_options(-O3)

add_executable(funtimes main.c component_labeling.c helperFun.c image_analysis.c component_analysis.c component_analysis.h)
