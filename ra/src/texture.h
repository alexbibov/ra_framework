#ifndef RA_TEXTURE_H
#define RA_TEXTURE_H

#include "buffer.h"

namespace ra {

    enum class RaTextureFormat
    {
        _float = RT_FORMAT_FLOAT,
        float2 = RT_FORMAT_FLOAT2,
        float3 = RT_FORMAT_FLOAT3,
        float4 = RT_FORMAT_FLOAT4,

        byte = RT_FORMAT_BYTE,
        byte2 = RT_FORMAT_BYTE2,
        byte3 = RT_FORMAT_BYTE3,
        byte4 = RT_FORMAT_BYTE4,

        unsigned_byte = RT_FORMAT_UNSIGNED_BYTE,
        unsigned_byte2 = RT_FORMAT_UNSIGNED_BYTE2,
        unsigned_byte3 = RT_FORMAT_UNSIGNED_BYTE3,
        unsigned_byte4 = RT_FORMAT_UNSIGNED_BYTE4,

        _short = RT_FORMAT_SHORT,
        short2 = RT_FORMAT_SHORT2,
        short3 = RT_FORMAT_SHORT3,
        short4 = RT_FORMAT_SHORT4,

        unsigned_short = RT_FORMAT_UNSIGNED_SHORT,
        unsigned_short2 = RT_FORMAT_UNSIGNED_SHORT2,
        unsigned_short3 = RT_FORMAT_UNSIGNED_SHORT3,
        unsigned_short4 = RT_FORMAT_UNSIGNED_SHORT4,

        _int = RT_FORMAT_INT,
        int2 = RT_FORMAT_INT2,
        int3 = RT_FORMAT_INT3,
        int4 = RT_FORMAT_INT4,

        unsinged_int = RT_FORMAT_UNSIGNED_INT,
        unsinged_int2 = RT_FORMAT_UNSIGNED_INT2,
        unsinged_int3 = RT_FORMAT_UNSIGNED_INT3,
        unsinged_int4 = RT_FORMAT_UNSIGNED_INT4,

        long_long = RT_FORMAT_LONG_LONG,
        long_long2 = RT_FORMAT_LONG_LONG2,
        long_long3 = RT_FORMAT_LONG_LONG3,
        long_long4 = RT_FORMAT_LONG_LONG4,

        unsigned_long_long = RT_FORMAT_UNSIGNED_LONG_LONG,
        unsigned_long_long2 = RT_FORMAT_UNSIGNED_LONG_LONG2,
        unsigned_long_long3 = RT_FORMAT_UNSIGNED_LONG_LONG3,
        unsigned_long_long4 = RT_FORMAT_UNSIGNED_LONG_LONG4,

        unsigned_bc1 = RT_FORMAT_UNSIGNED_BC1,
        unsigned_bc2 = RT_FORMAT_UNSIGNED_BC2,
        unsigned_bc3 = RT_FORMAT_UNSIGNED_BC3,
        unsigned_bc4 = RT_FORMAT_UNSIGNED_BC4,
        bc4 = RT_FORMAT_BC4,
        unsigned_bc5 = RT_FORMAT_UNSIGNED_BC5,
        bc5 = RT_FORMAT_BC5,
        unsigned_bc6h = RT_FORMAT_UNSIGNED_BC6H,
        bc6h = RT_FORMAT_BC6H,
        unsigned_bc7 = RT_FORMAT_UNSIGNED_BC7,

        custom = RT_FORMAT_USER
    };


    enum class RaTextureWrappingMode
    {
        repeat = RT_WRAP_REPEAT,
        clamp_to_edge = RT_WRAP_CLAMP_TO_EDGE,
        mirror = RT_WRAP_MIRROR,
        clamp_to_border = RT_WRAP_CLAMP_TO_BORDER
    };

    enum class RaTextureFilteringMode
    {
        nearest = RT_FILTER_NEAREST,
        linear = RT_FILTER_LINEAR
    };

    enum class RaTexturePixelFormat : uint8_t
    {
        r = 0,
        rg,
        rgb,
        rgba
    };


    class RaTexture : public RaContractWithRaContext, public RaEntity
    {
        RaTexture(RaContext const& context);

    protected:
        void initialize(uint32_t width, uint32_t height, RaTexturePixelFormat pixel_format,
            RaTextureFilteringMode filtering_mode, RaTextureWrappingMode wrapping_mode);

    private:
        std::shared_ptr<RTtexturesampler_api> m_sampler;
        std::unique_ptr<RaAbstractBuffer> m_buffer;
    };
}

#endif
