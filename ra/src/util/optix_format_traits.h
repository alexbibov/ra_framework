#ifndef RA_UTIL_OPTIX_FORMAT_TRAITS_H
#define RA_UTIL_OPTIX_FORMAT_TRAITS_H

#include <cstdint>
#include <optix.h>
#include <vector_types.h>

namespace ra::util {


    template<typename T> struct RaTextureFormatTraits;

    template<> struct RaTextureFormatTraits<float>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_FLOAT;
        static constexpr uint8_t size = 4;
        static constexpr uint8_t component_count = 1;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<float2>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_FLOAT2;
        static constexpr uint8_t size = 8;
        static constexpr uint8_t component_count = 2;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<float3>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_FLOAT3;
        static constexpr uint8_t size = 12;
        static constexpr uint8_t component_count = 3;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<float4>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_FLOAT4;
        static constexpr uint8_t size = 16;
        static constexpr uint8_t component_count = 4;
        static constexpr bool is_signed = true;
    };



    template<> struct RaTextureFormatTraits<char>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_BYTE;
        static constexpr uint8_t size = 1;
        static constexpr uint8_t component_count = 1;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<char2>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_BYTE2;
        static constexpr uint8_t size = 2;
        static constexpr uint8_t component_count = 2;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<char3>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_BYTE3;
        static constexpr uint8_t size = 3;
        static constexpr uint8_t component_count = 3;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<char4>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_BYTE4;
        static constexpr uint8_t size = 4;
        static constexpr uint8_t component_count = 4;
        static constexpr bool is_signed = true;
    };



    template<> struct RaTextureFormatTraits<unsigned char>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_BYTE;
        static constexpr uint8_t size = 4;
        static constexpr uint8_t component_count = 1;
        static constexpr bool is_signed = false;
    };

    template<> struct RaTextureFormatTraits<uchar2>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_BYTE2;
        static constexpr uint8_t size = 8;
        static constexpr uint8_t component_count = 2;
        static constexpr bool is_signed = false;
    };

    template<> struct RaTextureFormatTraits<uchar3>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_BYTE3;
        static constexpr uint8_t size = 12;
        static constexpr uint8_t component_count = 3;
        static constexpr bool is_signed = false;
    };

    template<> struct RaTextureFormatTraits<uchar4>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_BYTE4;
        static constexpr uint8_t size = 16;
        static constexpr uint8_t component_count = 4;
        static constexpr bool is_signed = false;
    };



    template<> struct RaTextureFormatTraits<short>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_SHORT;
        static constexpr uint8_t size = 2;
        static constexpr uint8_t component_count = 1;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<short2>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_SHORT2;
        static constexpr uint8_t size = 4;
        static constexpr uint8_t component_count = 2;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<short3>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_SHORT3;
        static constexpr uint8_t size = 6;
        static constexpr uint8_t component_count = 3;
        static constexpr bool is_signed = true;
    };

    template<> struct RaTextureFormatTraits<short4>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_SHORT4;
        static constexpr uint8_t size = 8;
        static constexpr uint8_t component_count = 4;
        static constexpr bool is_signed = true;
    };



    template<> struct RaTextureFormatTraits<unsigned short>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_SHORT;
        static constexpr uint8_t size = 4;
        static constexpr uint8_t component_count = 1;
        static constexpr bool is_signed = false;
    };

    template<> struct RaTextureFormatTraits<ushort2>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_SHORT2;
        static constexpr uint8_t size = 8;
        static constexpr uint8_t component_count = 2;
        static constexpr bool is_signed = false;
    };

    template<> struct RaTextureFormatTraits<ushort3>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_SHORT3;
        static constexpr uint8_t size = 12;
        static constexpr uint8_t component_count = 3;
        static constexpr bool is_signed = false;
    };

    template<> struct RaTextureFormatTraits<ushort4>
    {
        static constexpr uint32_t optix_format = RT_FORMAT_UNSIGNED_SHORT4;
        static constexpr uint8_t size = 16;
        static constexpr uint8_t component_count = 4;
        static constexpr bool is_signed = false;
    };
}

#endif
