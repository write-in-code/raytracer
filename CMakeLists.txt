cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 20)
file(GLOB RAYTRACER_SRC
     "src/*.h"
     "src/*.cpp"
)

add_executable(raytracer ${RAYTRACER_SRC})
target_precompile_headers(raytracer PRIVATE "src/Pch.h")

find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
target_link_libraries(raytracer glm::glm-header-only SDL2::SDL2)