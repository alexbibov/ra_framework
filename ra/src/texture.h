#ifndef RA_TEXTURE_H
#define RA_TEXTURE_H

#include "buffer.h"

namespace ra {

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


template<typename T>
class RaTexture : public RaContractWithRaContext, public RaEntity
{
    RaTexture(RaContext const& context);

protected:
    void initialize(uint32_t width, uint32_t height, RaTexturePixelFormat pixel_format,
        RaTextureFilteringMode filtering_mode, RaTextureWrappingMode wrapping_mode);

private:
    std::shared_ptr<RTtexturesampler_api> m_sampler;
    std::unique_ptr<RaBuffer<T>> m_buffer;
};
}

#endif
