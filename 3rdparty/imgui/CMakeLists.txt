cmake_minimum_required(VERSION 3.2)
project(imgui)

set(HEADERS_CXX_FILES
        imgui.h
        imconfig.h
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        misc/cpp/imgui_stdlib.h
)

set(SOURCES_CXX_FILES
        imgui.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_demo.cpp
        misc/cpp/imgui_stdlib.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${HEADERS_CXX_FILES}
        ${SOURCES_CXX_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ./)