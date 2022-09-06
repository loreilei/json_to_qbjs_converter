cmake_minimum_required(VERSION 3.23)

project(json_to_qbjs_converter VERSION 0.1.0)

find_package(Qt5 COMPONENTS Core REQUIRED QUIET)

add_executable(json_to_qbjs_converter main.cpp)

set_property(TARGET json_to_qbjs_converter PROPERTY CXX_STANDARD 17)

target_link_libraries(json_to_qbjs_converter
    PRIVATE
        Qt5::Core
)
