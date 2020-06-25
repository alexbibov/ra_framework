cmake_minimum_required(VERSION 3.15.2)
project(ra-target)

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
set(_64_bit_build TRUE)
else()
set(_64_bit_build FALSE)
endif()

if(NOT _64_bit_build)
message(FATAL_ERROR "Only 64-bit build is supported")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/_3rd_party/lua)
add_subdirectory(${CMAKE_SOURCE_DIR}/ra)
add_subdirectory(${CMAKE_SOURCE_DIR}/ra_processor)

