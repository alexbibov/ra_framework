cmake_minimum_required(VERSION 3.15.2)
set(_project_name ra_processor)
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

add_executable(${_target} ${_sources_list})

source_group(TREE ${_src_root_path} PREFIX "Source files" FILES ${_sources_list})
source_group(TREE ${_src_root_path} PREFIX "Header files" FILES ${_headers_list})

target_link_directories(${_target} PUBLIC "${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/")
target_link_directories(${_target} PRIVATE "${CMAKE_BINARY_DIR}/ra/$<CONFIG>")
target_link_directories(${_target} PRIVATE "${CMAKE_BINARY_DIR}/_3rd_party/lua/$<CONFIG>")
target_link_libraries(${_target} PRIVATE "cuda.lib" "cudart.lib")
target_link_libraries(${_target} PRIVATE "${optix_LIBRARY}")
target_link_libraries(${_target} PRIVATE ra_library lua)

target_include_directories(${_target} PRIVATE "${CMAKE_SOURCE_DIR}/_3rd_party/tclap/include")
target_include_directories(${_target} PRIVATE "${CMAKE_SOURCE_DIR}/_3rd_party/sol2")
target_include_directories(${_target} PRIVATE "${CMAKE_SOURCE_DIR}/_3rd_party/lua")
target_include_directories(${_target} PRIVATE "${CMAKE_SOURCE_DIR}/ra/src")
target_include_directories(${_target} PRIVATE "${OptiX_INCLUDE}")
target_include_directories(${_target} PRIVATE "${CUDA_TOOLKIT_ROOT_DIR}/include")

file(COPY "${optix_DLL}" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/")