#ifndef RA_UTIL_OPTIX_FORMAT_TRAITS_H
#define RA_UTIL_OPTIX_FORMAT_TRAITS_H

#include <cstdint>
#include <optix.h>
#include <vector_types.h>

namespace ra::util {

enum class RaColorDataCompression
{
    unsigned_bc1,
    unsigned_bc2,
    unsigned_bc3,
    unsigned_bc4,
    bc4,
    unsigned_bc5,
    bc5,
    unsigned_bc6h,
    bc6h,
    unsigned_bc7,
    none
};

template<typename T, RaColorDataCompression compression = RaColorDataCompression::none>
struct RaOptiXFormatTraits
{
    static constexpr RTformat optix_format = RT_FORMAT_USER;
    static constexpr uint32_t size = sizeof(T);
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<float>
{
    static constexpr RTformat optix_format = RT_FORMAT_FLOAT;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<float2>
{
    static constexpr RTformat optix_format = RT_FORMAT_FLOAT2;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<float3>
{
    static constexpr RTformat optix_format = RT_FORMAT_FLOAT3;
    static constexpr uint32_t size = 12;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<float4>
{
    static constexpr RTformat optix_format = RT_FORMAT_FLOAT4;
    static constexpr uint32_t size = 16;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<char>
{
    static constexpr RTformat optix_format = RT_FORMAT_BYTE;
    static constexpr uint32_t size = 1;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<char2>
{
    static constexpr RTformat optix_format = RT_FORMAT_BYTE2;
    static constexpr uint32_t size = 2;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<char3>
{
    static constexpr RTformat optix_format = RT_FORMAT_BYTE3;
    static constexpr uint32_t size = 3;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<char4>
{
    static constexpr RTformat optix_format = RT_FORMAT_BYTE4;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<unsigned char>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_BYTE;
    static constexpr uint32_t size = 1;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<uchar2>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_BYTE2;
    static constexpr uint32_t size = 2;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<uchar3>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_BYTE3;
    static constexpr uint32_t size = 3;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<uchar4>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_BYTE4;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<short>
{
    static constexpr RTformat optix_format = RT_FORMAT_SHORT;
    static constexpr uint32_t size = 2;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<short2>
{
    static constexpr RTformat optix_format = RT_FORMAT_SHORT2;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<short3>
{
    static constexpr RTformat optix_format = RT_FORMAT_SHORT3;
    static constexpr uint32_t size = 6;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<short4>
{
    static constexpr RTformat optix_format = RT_FORMAT_SHORT4;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<unsigned short>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_SHORT;
    static constexpr uint32_t size = 2;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<ushort2>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_SHORT2;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<ushort3>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_SHORT3;
    static constexpr uint32_t size = 6;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<ushort4>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_SHORT4;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<int>
{
    static constexpr RTformat optix_format = RT_FORMAT_INT;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<int2>
{
    static constexpr RTformat optix_format = RT_FORMAT_INT2;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<int3>
{
    static constexpr RTformat optix_format = RT_FORMAT_INT3;
    static constexpr uint32_t size = 12;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<int4>
{
    static constexpr RTformat optix_format = RT_FORMAT_INT4;
    static constexpr uint32_t size = 16;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<unsigned int>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_INT;
    static constexpr uint32_t size = 4;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<uint2>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_INT2;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<uint3>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_INT3;
    static constexpr uint32_t size = 12;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<uint4>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_INT4;
    static constexpr uint32_t size = 16;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<long long>
{
    static constexpr RTformat optix_format = RT_FORMAT_LONG_LONG;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<longlong2>
{
    static constexpr RTformat optix_format = RT_FORMAT_LONG_LONG2;
    static constexpr uint32_t size = 16;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<longlong3>
{
    static constexpr RTformat optix_format = RT_FORMAT_LONG_LONG3;
    static constexpr uint32_t size = 24;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<longlong4>
{
    static constexpr RTformat optix_format = RT_FORMAT_LONG_LONG4;
    static constexpr uint32_t size = 32;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = true;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



template<> struct RaOptiXFormatTraits<unsigned long long>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_LONG_LONG;
    static constexpr uint32_t size = 8;
    static constexpr uint8_t component_count = 1;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<ulonglong2>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_LONG_LONG2;
    static constexpr uint32_t size = 16;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<ulonglong3>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_LONG_LONG3;
    static constexpr uint32_t size = 24;
    static constexpr uint8_t component_count = 3;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};

template<> struct RaOptiXFormatTraits<ulonglong4>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_LONG_LONG4;
    static constexpr uint32_t size = 32;
    static constexpr uint8_t component_count = 4;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::none;
};



/*template<> struct RaOptiXFormatTraits<uchar3, RaColorDataCompression::unsigned_bc1>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_BC1;
    static constexpr uint32_t size = 2;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::unsigned_bc1;
};

template<> struct RaOptiXFormatTraits<uchar4, RaColorDataCompression::unsigned_bc2>
{
    static constexpr RTformat optix_format = RT_FORMAT_UNSIGNED_BC2;
    static constexpr uint32_t size = 2;
    static constexpr uint8_t component_count = 2;
    static constexpr bool is_signed = false;
    RaColorDataCompression compression = RaColorDataCompression::unsigned_bc2;
};*/
}

#endif
