cmake_minimum_required(VERSION 3.2)

project(gin)

set(CMAKE_CXX_STANDARD 17)
set(EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

if (MSVC)
    add_compile_options(/arch:AVX)
else()
    add_compile_options(-mavx)
endif()

option(GIN_BUILD_BENCH "Build gin benchmark" ON)
option(GIN_BUILD_EDITOR "Build gin editor" ON)
option(GIN_BUILD_EDITOR_WITH_WAYLAND "Build gin editor using wayland" OFF)

if (GIN_BUILD_EDITOR_WITH_WAYLAND)
    set(GLFW_BUILD_WAYLAND ON)
    set(GLFW_BUILD_X11 OFF)
else()
    set(GLFW_BUILD_WAYLAND OFF)
    set(GLFW_BUILD_X11 ON)
endif()

add_subdirectory(${EXTERNAL_PATH}/xsimd)
add_subdirectory(${EXTERNAL_PATH}/eigen)
add_subdirectory(${EXTERNAL_PATH}/nanobench)
add_subdirectory(${EXTERNAL_PATH}/nativefiledialog-extended-1.1.0)
add_subdirectory(${EXTERNAL_PATH}/FastNoise2)
add_subdirectory(${EXTERNAL_PATH}/glfw)
add_subdirectory(${EXTERNAL_PATH}/imgui)
add_subdirectory(${EXTERNAL_PATH}/imgui-node-editor)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE gin_library_src "library/*.cpp" "library/*.hpp")
file(GLOB_RECURSE gin_editor_src "editor/*.cpp" "editor/*.hpp")
file(GLOB_RECURSE gin_bench_src "bench/*.cpp" "bench/*.hpp")

file(GLOB files "modules/*")
foreach(file ${files})

IF(IS_DIRECTORY ${file})
file(RELATIVE_PATH name ${CMAKE_CURRENT_SOURCE_DIR}/modules ${file})

list(APPEND includes_list "#include <${name}/${name}.hpp>")

string(SUBSTRING ${name} 0 1 first)
string(TOUPPER ${first} first)
string(REGEX REPLACE "^.(.*)" "${first}\\1" pascal_name "${name}")

list(APPEND initialize_module_list "\tInitialize${pascal_name}Module()")
list(APPEND uninitialize_module_list "\tUninitialize${pascal_name}Module()")

file(GLOB_RECURSE gin_module_${name} "modules/${name}/*.cpp" "modules/${name}/*.hpp")
list(APPEND gin_modules_src ${gin_module_${name}})
ENDIF()

endforeach()

list(JOIN includes_list "\n" includes)
list(JOIN initialize_module_list "\;\n" initialize_module)
list(JOIN uninitialize_module_list "\;\n" uninitialize_module)

set(module_file_src "#pragma once\n\n${includes}\n\ninline void Initialize() \{\n${initialize_module}\;\n\}\n\ninline void Uninitialize() \{\n${uninitialize_module}\;\n\}")

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/modules/module.hpp ${module_file_src})

add_library(gin_library STATIC ${gin_library_src} ${gin_modules_src} ${CMAKE_CURRENT_SOURCE_DIR}/modules/module.hpp
        modules/signed/nodes/3d/sampler.hpp
        modules/signed/nodes/3d/sampler.cpp)

target_include_directories(gin_library PUBLIC library/include modules)
target_link_libraries(gin_library xsimd FastNoise)

if(GIN_BUILD_EDITOR)
    add_executable(gin_editor ${gin_editor_src})
    target_link_libraries(gin_editor gin_library Eigen3::Eigen nfd glfw imgui imgui_node_editor)
    target_include_directories(gin_editor PRIVATE ${EXTERNAL_PATH}/glad/include)
endif()

if(GIN_BUILD_BENCH)
    add_executable(gin_bench ${gin_bench_src}
            library/include/gin/field/sampler.hpp
            library/include/gin/math/type.hpp
            library/include/gin/field/field.hpp
            library/include/gin/field/constantfield.hpp
            library/include/gin/sdf/primitive.hpp
            modules/geometry/geometry.hpp
            modules/geometry/geometry.cpp
            modules/geometry/nodes/point.hpp
    )
    target_link_libraries(gin_bench gin_library nanobench)
endif ()