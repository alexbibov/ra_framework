cmake_minimum_required(VERSION 3.15.2)
set(_project_name ra_library)
project(${_project_name})

set(_src_root_path ${PROJECT_SOURCE_DIR})
set(_target ra_library)

file(
    GLOB_RECURSE _cpp_sources_list
    CONFIGURE_DEPENDS
    "${_src_root_path}/src/*.cpp"
)

file(
    GLOB_RECURSE _cuda_sources_list
    CONFIGURE_DEPENDS
    "${_src_root_path}/src/*.cu"
)

file(
    GLOB_RECURSE _headers_list
    "${_src_root_path}/src/*.h"
)

add_library(${_target} STATIC ${_cpp_sources_list} ${_headers_list} ${_cuda_sources_list})
source_group(TREE ${_src_root_path}/src PREFIX "Source files" FILES ${_cpp_sources_list})
source_group(TREE ${_src_root_path}/src PREFIX "Header files" FILES ${_headers_list})
source_group(TREE ${_src_root_path}/src PREFIX "CUDA sources" FILES ${_cuda_sources_list})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${_src_root_path}/")
set(${FIND_LIBRARY_USE_LIB64_PATHS} TRUE)
set(${FIND_LIBRARY_USE_LIB32_PATHS} FALSE)

if(MSVC)
    target_compile_options(${_target} PRIVATE /bigobj)
    target_compile_definitions(${_target} PRIVATE UNICODE)
endif()

find_package(CUDA 10.1 REQUIRED)
find_package(OptiX 6.5.0 REQUIRED)
find_package(Python COMPONENTS Interpreter REQUIRED)

target_include_directories(${_target} PRIVATE "${_src_root_path}/src")
target_include_directories(${_target} PUBLIC "${CMAKE_SOURCE_DIR}/_3rd_party/")
target_include_directories(${_target} PUBLIC "${CMAKE_SOURCE_DIR}/_3rd_party/lua")
target_include_directories(${_target} PUBLIC "${CUDA_TOOLKIT_ROOT_DIR}/include")
target_include_directories(${_target} PUBLIC "${OptiX_INCLUDE}")


target_link_directories(${_target} INTERFACE "${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/")
target_link_directories(${_target} INTERFACE "${CMAKE_BINARY_DIR}/_3rd_party/lua/$<CONFIG>")
target_link_libraries(${_target} INTERFACE "cuda.lib" "cudart.lib")
target_link_libraries(${_target} INTERFACE "${optix_LIBRARY}")
target_link_libraries(${_target} INTERFACE lua)
target_link_libraries(${_target} INTERFACE )

add_custom_command(
    TARGET ${_target}
    PRE_BUILD
    COMMAND ${Python_EXECUTABLE} "${CMAKE_SOURCE_DIR}/compile_ptx.py" --sources "\"${_cuda_sources_list}\"" --ccbin \"${CUDA_HOST_COMPILER}\" --nvcc "\"${CUDA_TOOLKIT_ROOT_DIR}/bin/nvcc.exe\"" --nvcc-opts \"-arch=compute_60 -code=sm_60 -m64\" --include-directories "\"${OptiX_INSTALL_DIR}/include;${_src_root_path}/src/\"" --header "\"${_src_root_path}/src/ptx.h\"" --settings "\"${CMAKE_SOURCE_DIR}/settings.json\"" --build-path \"${CMAKE_BINARY_DIR}\"
    COMMENT "Compiling OptiX Shaders"
)













