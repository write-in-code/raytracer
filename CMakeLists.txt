cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 20)
file(GLOB RAYTRACER_SRC
     "src/*.h"
     "src/*.cpp"
)

add_executable(raytracer ${RAYTRACER_SRC})
target_precompile_headers(raytracer PRIVATE "src/Pch.h")

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(raytracer PRIVATE glm::glm-header-only SDL2::SDL2 SDL2_image::SDL2_image)