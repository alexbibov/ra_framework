cmake_minimum_required(VERSION 3.15.2)
set(_project_name lua)
project(${_project_name})

set(_src_root_path ${PROJECT_SOURCE_DIR})
set(_target ${_project_name})


file(
    GLOB_RECURSE _sources_list
    CONFIGURE_DEPENDS TRUE
    "${_src_root_path}/*.c" "${_src_root_path}/*.cpp"
)

file(
    GLOB_RECURSE _headers_list
    CONFIGURE_DEPENDS TRUE
    "${_src_root_path}/*.h" "${_src_root_path}/*.hpp"
)

add_library(${_target} STATIC ${_sources_list} ${_headers_list})

source_group(TREE ${_src_root_path} PREFIX "Source files" FILES ${_sources_list})
source_group(TREE ${_src_root_path} PREFIX "Header files" FILES ${_headers_list})