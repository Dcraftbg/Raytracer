cmake_minimum_required(VERSION 3.12)
project(Raytracer)

file(GLOB_RECURSE RAYTRACER_SOURCES CONFIGURE_DEPENDS  Raytracer/src/*.cpp Raytracer/src/*.h)
add_executable(Raytracer ${RAYTRACER_SOURCES})
