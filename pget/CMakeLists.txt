cmake_minimum_required(VERSION 3.15.2)
set(_project_name pget_extension)
project(${_project_name})

set(_src_root_path ${PROJECT_SOURCE_DIR})
set(_target ${_project_name})

file(GLOB_RECURSE _cpp_source_list
     CONFIGURE_DEPENDS
     "${_src_root_path}/*.cpp")

file(GLOB_RECURSE _header_source_list
     CONFIGURE_DEPENDS
     "${_src_root_path}/*.h" "${_src_root_path}/*.hpp")

file(GLOB_RECURSE _cuda_source_list
     CONFIGURE_DEPENDS
     "${_src_root_path}/*.cuh" "${_src_root_path}/*.cu")

add_library(${_target} SHARED ${_cpp_source_list} ${_header_source_list} ${_cuda_source_list})

source_group(TREE ${_src_root_path} PREFIX "Source files" FILES ${_cpp_source_list})
source_group(TREE ${_src_root_path} PREFIX "Header files" FILES ${_header_source_list})
source_group(TREE ${_src_root_path} PREFIX "CUDA sources" FILES ${_cuda_source_list})

target_link_directories(${_target} PRIVATE "${CMAKE_BINARY_DIR}/ra/$<CONFIG>")
target_link_libraries(${_target} PRIVATE ra_library)

target_include_directories(${_target} PRIVATE "${CMAKE_SOURCE_DIR}/ra/src")

set_property(TARGET ${_target} PROPERTY OUTPUT_NAME "${_project_name}.mex64")

file(COPY "${optix_DLL}" DESTINATION "${LIBRARY_OUTPUT_PATH}/")

message(${_cuda_source_list})

add_custom_command(
    TARGET ${_target}
    PRE_BUILD
    COMMAND ${Python_EXECUTABLE} "${CMAKE_SOURCE_DIR}/compile_ptx.py" --sources "\"${_cuda_source_list}\"" --ccbin \"${CUDA_HOST_COMPILER}\" --nvcc "\"${CUDA_TOOLKIT_ROOT_DIR}/bin/nvcc.exe\"" --nvcc-opts \"-arch=compute_60 -code=sm_60 -m64\" --include-directories "\"${OptiX_INSTALL_DIR}/include;${_src_root_path}/\"" --header "\"${_src_root_path}/ptx.h\"" --settings "\"${CMAKE_SOURCE_DIR}/settings.json\"" --build-path \"${CMAKE_BINARY_DIR}\"
    COMMENT "Compiling OptiX Shaders"
)