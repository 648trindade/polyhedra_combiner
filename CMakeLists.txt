cmake_minimum_required(VERSION 3.18)
project(polyhedra_combiner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${polyhedra_combiner_SOURCE_DIR}/cmake")

add_library(
    polyhedra
        Vertex.h
        Vertex.cpp
        Edge.cpp
        Edge.h
        Face.cpp
        Face.h
        Solid.cpp
        Solid.h
        BoundingBox.cpp
        BoundingBox.h
        numeric.h)

add_library(
    utils
        input_parser.h
        input_parser.cpp
        intersection.cpp
        intersection.h
)
target_include_directories(utils PRIVATE include)

add_executable(polyhedra_combiner main.cpp)
target_link_libraries(
    polyhedra_combiner
        utils
        polyhedra
)

add_subdirectory(tests)