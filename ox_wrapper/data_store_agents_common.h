#ifndef OX_WRAPPER_DATA_STORE_AGENTS_COMMON_H
#define OX_WRAPPER_DATA_STORE_AGENTS_COMMON_H

#include "ox_wrapper/buffer.h"
#include "ox_wrapper/ray_payloads.h"

#include <iostream>

namespace ox_wrapper {
namespace data_store_agents {

template<OxBasicBufferFormat buffer_format>
struct DataReadWriteHelper;

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::FLOAT>
{
    static void read(std::istream& input_stream, float* dst, size_t len)
    {
        input_stream.read(reinterpret_cast<char*>(dst), sizeof(float)*len);
    }

    static void write(std::ostream& output_stream, float const* src, size_t len)
    {
        output_stream.write(reinterpret_cast<char const*>(src), sizeof(float)*len);
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::FLOAT2>
{
    static void read(std::istream& input_stream, float2* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float aux[2];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(float) * 2);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
        }

    }

    static void write(std::ostream& output_stream, float2 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float aux[2] = { src[i].x, src[i].y };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(float) * 2);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::FLOAT3>
{
    static void read(std::istream& input_stream, float3* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float aux[3];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(float) * 3);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
        }

    }

    static void write(std::ostream& output_stream, float3 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float aux[3] = { src[i].x, src[i].y, src[i].z };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(float) * 3);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::FLOAT4>
{
    static void read(std::istream& input_stream, float4* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float aux[4];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(float) * 4);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
            dst[i].w = aux[3];
        }

    }

    static void write(std::ostream& output_stream, float4 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float aux[4] = { src[i].x, src[i].y, src[i].z, src[i].w };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(float) * 4);
        }
    }
};


template<>
struct DataReadWriteHelper<OxBasicBufferFormat::INT>
{
    static void read(std::istream& input_stream, int* dst, size_t len)
    {
        input_stream.read(reinterpret_cast<char*>(dst), sizeof(int)*len);
    }

    static void write(std::ostream& output_stream, int const* src, size_t len)
    {
        output_stream.write(reinterpret_cast<char const*>(src), sizeof(int)*len);
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::INT2>
{
    static void read(std::istream& input_stream, int2* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            int aux[2];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(int) * 2);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
        }

    }

    static void write(std::ostream& output_stream, int2 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            int aux[2] = { src[i].x, src[i].y };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(int) * 2);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::INT3>
{
    static void read(std::istream& input_stream, int3* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            int aux[3];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(int) * 3);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
        }

    }

    static void write(std::ostream& output_stream, int3 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            int aux[3] = { src[i].x, src[i].y, src[i].z };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(int) * 3);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::INT4>
{
    static void read(std::istream& input_stream, int4* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            int aux[4];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(int) * 4);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
            dst[i].w = aux[3];
        }

    }

    static void write(std::ostream& output_stream, int4 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            int aux[4] = { src[i].x, src[i].y, src[i].z, src[i].w };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(int) * 4);
        }
    }
};


template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UINT>
{
    static void read(std::istream& input_stream, unsigned int* dst, size_t len)
    {
        input_stream.read(reinterpret_cast<char*>(dst), sizeof(unsigned int)*len);
    }

    static void write(std::ostream& output_stream, unsigned int const* src, size_t len)
    {
        output_stream.write(reinterpret_cast<char const*>(src), sizeof(unsigned int)*len);
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UINT2>
{
    static void read(std::istream& input_stream, uint2* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned int aux[2];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(unsigned int) * 2);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
        }

    }

    static void write(std::ostream& output_stream, uint2 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned int aux[2] = { src[i].x, src[i].y };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(unsigned int) * 2);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UINT3>
{
    static void read(std::istream& input_stream, uint3* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned int aux[3];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(unsigned int) * 3);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
        }

    }

    static void write(std::ostream& output_stream, uint3 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned int aux[3] = { src[i].x, src[i].y, src[i].z };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(unsigned int) * 3);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UINT4>
{
    static void read(std::istream& input_stream, uint4* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned int aux[4];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(unsigned int) * 4);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
            dst[i].w = aux[3];
        }

    }

    static void write(std::ostream& output_stream, uint4 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned int aux[4] = { src[i].x, src[i].y, src[i].z, src[i].w };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(unsigned int) * 4);
        }
    }
};


template<>
struct DataReadWriteHelper<OxBasicBufferFormat::CHAR>
{
    static void read(std::istream& input_stream, char* dst, size_t len)
    {
        input_stream.read(dst, len);
    }

    static void write(std::ostream& output_stream, char const* src, size_t len)
    {
        output_stream.write(src, len);
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::CHAR2>
{
    static void read(std::istream& input_stream, char2* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            char aux[2];
            input_stream.read(aux, 2);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
        }

    }

    static void write(std::ostream& output_stream, char2 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            char aux[2] = { src[i].x, src[i].y };
            output_stream.write(aux, 2);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::CHAR3>
{
    static void read(std::istream& input_stream, char3* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            char aux[3];
            input_stream.read(aux, 3);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
        }

    }

    static void write(std::ostream& output_stream, char3 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            char aux[3] = { src[i].x, src[i].y, src[i].z };
            output_stream.write(aux, 3);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::CHAR4>
{
    static void read(std::istream& input_stream, char4* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            char aux[4];
            input_stream.read(aux, 4);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
            dst[i].w = aux[3];
        }

    }

    static void write(std::ostream& output_stream, char4 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            char aux[4] = { src[i].x, src[i].y, src[i].z, src[i].w };
            output_stream.write(aux, 4);
        }
    }
};


template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UCHAR>
{
    static void read(std::istream& input_stream, unsigned char* dst, size_t len)
    {
        input_stream.read(reinterpret_cast<char*>(dst), sizeof(unsigned char)*len);
    }

    static void write(std::ostream& output_stream, unsigned char const* src, size_t len)
    {
        output_stream.write(reinterpret_cast<char const*>(src), sizeof(unsigned char)*len);
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UCHAR2>
{
    static void read(std::istream& input_stream, uchar2* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned char aux[2];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(unsigned char) * 2);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
        }

    }

    static void write(std::ostream& output_stream, uchar2 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned char aux[2] = { src[i].x, src[i].y };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(unsigned char) * 2);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UCHAR3>
{
    static void read(std::istream& input_stream, uchar3* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned char aux[3];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(unsigned char) * 3);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
        }

    }

    static void write(std::ostream& output_stream, uchar3 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned char aux[3] = { src[i].x, src[i].y, src[i].z };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(unsigned char) * 3);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::UCHAR4>
{
    static void read(std::istream& input_stream, uchar4* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned char aux[4];
            input_stream.read(reinterpret_cast<char*>(aux), sizeof(unsigned char) * 4);
            dst[i].x = aux[0];
            dst[i].y = aux[1];
            dst[i].z = aux[2];
            dst[i].w = aux[3];
        }

    }

    static void write(std::ostream& output_stream, uchar4 const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            unsigned char aux[4] = { src[i].x, src[i].y, src[i].z, src[i].w };
            output_stream.write(reinterpret_cast<char const*>(aux), sizeof(unsigned char) * 4);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD>
{
    static void read(std::istream& input_stream, OxRayRadiancePayload* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            for (uint32_t j = 0; j < constants::max_spectra_pairs_supported; ++j)
            {
                float spectral_pair[2];
                input_stream.read(reinterpret_cast<char*>(spectral_pair), sizeof(float) * 2);
                dst[i].spectral_radiance[j].x = spectral_pair[0];
                dst[i].spectral_radiance[j].y = spectral_pair[1];
            }

            float depth[2];
            input_stream.read(reinterpret_cast<char*>(depth), sizeof(float) * 2);
            dst[i].depth.x = depth[0];
            dst[i].depth.y = depth[1];

            unsigned int tracing_depth_and_aux[4];
            input_stream.read(reinterpret_cast<char*>(tracing_depth_and_aux), sizeof(unsigned int) * 4);
            dst[i].tracing_depth_and_aux.x = tracing_depth_and_aux[0];
            dst[i].tracing_depth_and_aux.y = tracing_depth_and_aux[1];
            dst[i].tracing_depth_and_aux.z = tracing_depth_and_aux[2];
            dst[i].tracing_depth_and_aux.w = tracing_depth_and_aux[3];
        }
    }

    static void write(std::ostream& output_stream, OxRayRadiancePayload const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            for (uint32_t j = 0; j < constants::max_spectra_pairs_supported; ++j)
            {
                float spectral_pair[2] = { src[i].spectral_radiance[j].x, src[i].spectral_radiance[j].y };
                output_stream.write(reinterpret_cast<char const*>(spectral_pair), sizeof(float) * 2);
            }

            float depth[2] = { src[i].depth.x, src[i].depth.y };
            output_stream.write(reinterpret_cast<char const*>(depth), sizeof(float) * 2);

            unsigned int tracing_depth_and_aux[4] =
            { src[i].tracing_depth_and_aux.x, src[i].tracing_depth_and_aux.y,
            src[i].tracing_depth_and_aux.z, src[i].tracing_depth_and_aux.w };
            output_stream.write(reinterpret_cast<char const*>(tracing_depth_and_aux), sizeof(unsigned int) * 4);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE>
{
    static void read(std::istream& input_stream, OxRayRadiancePayloadSimple* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float spectral_pair[2];
            input_stream.read(reinterpret_cast<char*>(spectral_pair), sizeof(float) * 2);
            dst[i].spectral_radiance.x = spectral_pair[0];
            dst[i].spectral_radiance.y = spectral_pair[1];

            float depth[2];
            input_stream.read(reinterpret_cast<char*>(depth), sizeof(float) * 2);
            dst[i].depth.x = depth[0];
            dst[i].depth.y = depth[1];

            unsigned int tracing_depth_and_aux[4];
            input_stream.read(reinterpret_cast<char*>(tracing_depth_and_aux), sizeof(unsigned int) * 4);
            dst[i].tracing_depth_and_aux.x = tracing_depth_and_aux[0];
            dst[i].tracing_depth_and_aux.y = tracing_depth_and_aux[1];
            dst[i].tracing_depth_and_aux.z = tracing_depth_and_aux[2];
            dst[i].tracing_depth_and_aux.w = tracing_depth_and_aux[3];
        }
    }

    static void write(std::ostream& output_stream, OxRayRadiancePayloadSimple const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float spectral_pair[2] = { src[i].spectral_radiance.x, src[i].spectral_radiance.y };
            output_stream.write(reinterpret_cast<char const*>(spectral_pair), sizeof(float) * 2);

            float depth[2] = { src[i].depth.x, src[i].depth.y };
            output_stream.write(reinterpret_cast<char const*>(depth), sizeof(float) * 2);

            unsigned int tracing_depth_and_aux[4] =
            { src[i].tracing_depth_and_aux.x, src[i].tracing_depth_and_aux.y,
            src[i].tracing_depth_and_aux.z, src[i].tracing_depth_and_aux.w };
            output_stream.write(reinterpret_cast<char const*>(tracing_depth_and_aux), sizeof(unsigned int) * 4);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC>
{
    static void read(std::istream& input_stream, OxRayRadiancePayloadMonochromatic* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            float spectral_radiance;
            input_stream.read(reinterpret_cast<char*>(&spectral_radiance), sizeof(float));
            dst[i].spectral_radiance = spectral_radiance;

            float depth[2];
            input_stream.read(reinterpret_cast<char*>(depth), sizeof(float) * 2);
            dst[i].depth.x = depth[0];
            dst[i].depth.y = depth[1];

            unsigned int tracing_depth_and_aux[4];
            input_stream.read(reinterpret_cast<char*>(tracing_depth_and_aux), sizeof(unsigned int) * 4);
            dst[i].tracing_depth_and_aux.x = tracing_depth_and_aux[0];
            dst[i].tracing_depth_and_aux.y = tracing_depth_and_aux[1];
            dst[i].tracing_depth_and_aux.z = tracing_depth_and_aux[2];
            dst[i].tracing_depth_and_aux.w = tracing_depth_and_aux[3];
        }
    }

    static void write(std::ostream& output_stream, OxRayRadiancePayloadMonochromatic const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            output_stream.write(reinterpret_cast<char const*>(&src[i].spectral_radiance), sizeof(float));

            float depth[2] = { src[i].depth.x, src[i].depth.y };
            output_stream.write(reinterpret_cast<char const*>(depth), sizeof(float) * 2);

            unsigned int tracing_depth_and_aux[4] =
            { src[i].tracing_depth_and_aux.x, src[i].tracing_depth_and_aux.y,
            src[i].tracing_depth_and_aux.z, src[i].tracing_depth_and_aux.w };
            output_stream.write(reinterpret_cast<char const*>(tracing_depth_and_aux), sizeof(unsigned int) * 4);
        }
    }
};

template<>
struct DataReadWriteHelper<OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD>
{
    static void read(std::istream& input_stream, OxRayOcclusionPayload* dst, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            input_stream.read(reinterpret_cast<char*>(&dst[i].is_occluded), sizeof(unsigned int));
            input_stream.read(reinterpret_cast<char*>(&dst[i].tracing_depth), sizeof(unsigned int));

            float depth[2];
            input_stream.read(reinterpret_cast<char*>(depth), sizeof(float) * 2);
            dst[i].depth.x = depth[0]; dst[i].depth.y = depth[1];
        }
    }

    static void write(std::ostream& output_stream, OxRayOcclusionPayload const* src, size_t len)
    {
        for (size_t i = 0U; i < len; ++i)
        {
            output_stream.write(reinterpret_cast<char const*>(&src[i].is_occluded), sizeof(unsigned int));
            output_stream.write(reinterpret_cast<char const*>(&src[i].tracing_depth), sizeof(unsigned int));

            float depth[2] = { src[i].depth.x, src[i].depth.y };
            output_stream.write(reinterpret_cast<char const*>(depth), sizeof(float) * 2);
        }
    }
};


}
}

#endif
