cmake_minimum_required(VERSION 3.24)
project(funTimes C)

set(CMAKE_C_STANDARD 17)
add_compile_options(-O3)
#add_compile_options(-lm)

add_executable(funtimes helperFun.c helperFun.h main.c component_labeling.c  image_analysis.c component_analysis.c component_helper.c filters.c edgeDetector.c kmean.c utility.c assignments/assignment1.c assignments/assignment2.c assignments/assignment3.c kmeanAccelerated.cu)
target_link_libraries(funtimes m)