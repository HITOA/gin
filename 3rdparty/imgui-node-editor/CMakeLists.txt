cmake_minimum_required(VERSION 3.2)
project(imgui_node_editor)

set(_imgui_node_editor_Sources
        crude_json.cpp
        crude_json.h
        imgui_bezier_math.h
        imgui_bezier_math.inl
        imgui_canvas.cpp
        imgui_canvas.cpp
        imgui_canvas.h
        imgui_canvas.h
        imgui_extra_math.h
        imgui_extra_math.inl
        imgui_node_editor_api.cpp
        imgui_node_editor_internal.h
        imgui_node_editor_internal.inl
        imgui_node_editor.cpp
        imgui_node_editor.h
        misc/imgui_node_editor.natvis
)

add_library(imgui_node_editor STATIC
        ${_imgui_node_editor_Sources}
)

target_include_directories(imgui_node_editor PUBLIC
        ${IMGUI_NODE_EDITOR_ROOT_DIR}
)

target_link_libraries(imgui_node_editor PUBLIC imgui)
target_include_directories(imgui_node_editor PUBLIC ./)