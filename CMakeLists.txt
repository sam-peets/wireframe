cmake_minimum_required(VERSION 3.15...3.31)

project(
	Pipeline
  VERSION 1.0
  LANGUAGES CXX)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(glm CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(Pipeline
  src/main.cpp
  src/triangle.cpp
  src/screen.cpp
  src/renderer.cpp
  src/model.cpp

  include/triangle.hpp
  include/screen.hpp
  include/renderer.hpp
  include/model.hpp
  )

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall -Wpedantic -DGLM_FORCE_SWIZZLE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -fomit-frame-pointer -flto -Wall -Wpedantic -DGLM_FORCE_SWIZZLE -DGLM_FORCE_AVX")

target_include_directories(Pipeline PRIVATE include)
target_link_libraries(Pipeline PRIVATE SDL2::SDL2main)
target_link_libraries(Pipeline PRIVATE SDL2::SDL2)
target_link_libraries(Pipeline PRIVATE glm::glm)
target_link_libraries(Pipeline PRIVATE m)