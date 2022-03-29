cmake_minimum_required(VERSION 3.18)
project(polyhedra_combiner)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)

add_library(polyhedra Vertex.h Vertex.cpp Edge.cpp Edge.h Contour.cpp Contour.h Face.cpp Face.h Solid.cpp Solid.h)

add_executable(polyhedra_combiner main.cpp input_parser.h input_parser.cpp)
target_include_directories(polyhedra_combiner PRIVATE include)
target_link_libraries(
    polyhedra_combiner
    polyhedra
)

add_executable(testVertex tests/testVertex.cpp)
target_link_libraries(testVertex PRIVATE Catch2::Catch2WithMain polyhedra)

#include(CTest)
include(Catch)
catch_discover_tests(testVertex)