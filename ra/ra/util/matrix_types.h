#ifndef OX_WRAPPER_UTIL_MATRIX_TYPES_H
#define OX_WRAPPER_UTIL_MATRIX_TYPES_H

namespace ra{ namespace util{

template<typename T> struct alignas(16) matrix_2x2_struct { T _11, _12, _21, _22; };
template<typename T> struct alignas(16) matrix_2x3_struct { T _11, _12, _13, _21, _22, _23; };
template<typename T> struct alignas(16) matrix_2x4_struct { T _11, _12, _13, _14, _21, _22, _23, _24; };
template<typename T> struct alignas(16) matrix_3x2_struct { T _11, _12, _21, _22, _31, _32; };
template<typename T> struct alignas(16) matrix_3x3_struct { T _11, _12, _13, _21, _22, _23, _31, _32, _33; };
template<typename T> struct alignas(16) matrix_3x4_struct { T _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34; };
template<typename T> struct alignas(16) matrix_4x2_struct { T _11, _12, _21, _22, _31, _32, _41, _42; };
template<typename T> struct alignas(16) matrix_4x3_struct { T _11, _12, _13, _21, _22, _23, _31, _32, _33, _41, _42, _43; };
template<typename T> struct alignas(16) matrix_4x4_struct { T _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; };

using mat2x2 = matrix_2x2_struct<float>;
using mat2x3 = matrix_2x3_struct<float>;
using mat2x4 = matrix_2x4_struct<float>;

using mat3x2 = matrix_3x2_struct<float>;
using mat3x3 = matrix_3x3_struct<float>;
using mat3x4 = matrix_3x4_struct<float>;

using mat4x2 = matrix_4x2_struct<float>;
using mat4x3 = matrix_4x3_struct<float>;
using mat4x4 = matrix_4x4_struct<float>;

}}

#endif
