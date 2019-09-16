# CMakeList.txt : CMake project for ray_tracing_in_one_weekend, include source and define
# project specific logic here.
#

set(TARGET_NAME ray_tracing_in_one_weekend)
project(${TARGET_NAME})

cmake_minimum_required (VERSION 3.8)



# Add source to this project's executable.
add_executable (${TARGET_NAME} "ray_tracing_in_one_weekend.cpp" "ray_tracing_in_one_weekend.hpp")

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# TODO: Add tests and install targets if needed.
