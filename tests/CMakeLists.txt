find_package(Catch REQUIRED)

file(GLOB files "test*.cpp")
foreach(file ${files})
    get_filename_component(test_name ${file} NAME_WE)
    set(test_name ${test_name}.run)
    add_executable(${test_name} ${file})
    target_link_libraries(
            ${test_name}
            PRIVATE
            utils
            polyhedra
    )
    target_include_directories(
        ${test_name}
        PRIVATE
            ${polyhedra_combiner_SOURCE_DIR}/include
            ${polyhedra_combiner_SOURCE_DIR}/src
    )
    catch_discover_tests(${test_name})
endforeach()