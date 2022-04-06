cmake_minimum_required(VERSION 3.18)
project(polyhedra_combiner)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)

add_library(
    polyhedra
        Vertex.h
        Vertex.cpp
        Edge.cpp
        Edge.h
        Contour.cpp
        Contour.h
        Face.cpp
        Face.h
        Solid.cpp
        Solid.h
        BoundingBox.cpp
        BoundingBox.h
)

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

include(Catch)

file(GLOB files "tests/*.cpp")
foreach(file ${files})
    get_filename_component(test_name ${file} NAME_WE)
    add_executable(${test_name} ${file})
    target_link_libraries(
        ${test_name}
        PRIVATE
            Catch2::Catch2WithMain
            utils
            polyhedra
    )
    catch_discover_tests(${test_name})
endforeach()