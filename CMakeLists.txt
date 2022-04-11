cmake_minimum_required(VERSION 3.18)
project(polyhedra_combiner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${polyhedra_combiner_SOURCE_DIR}/cmake")

add_library(
    polyhedra
        src/Vertex.h
        src/Vertex.cpp
        src/Edge.cpp
        src/Edge.h
        src/Face.cpp
        src/Face.h
        src/Solid.cpp
        src/Solid.h
        src/BoundingBox.cpp
        src/BoundingBox.h
        src/numeric.h)

add_library(
    utils
        src/input_parser.h
        src/input_parser.cpp
        src/intersection.cpp
        src/intersection.h
)
target_include_directories(utils PRIVATE include)

add_executable(polyhedra_combiner src/main.cpp)
target_link_libraries(
    polyhedra_combiner
        utils
        polyhedra
)

add_subdirectory(tests)