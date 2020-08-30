#ifndef RA_TEXTURE_H
#define RA_TEXTURE_H

#include "buffer.h"
#include "ra_fwd.h"

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


class RaTexture : public RaContractWithRaContext, public RaEntity
{
public:
    virtual RaAbstractBuffer& buffer() = 0;
    bool isValid() const override;

    void setFilteringMode(RaTextureFilteringMode minification_filter, RaTextureFilteringMode magnification_filter,
        RaTextureFilteringMode mipmapping_filter, float anisotropy = 1.f);

    void setWrappingMode(RaTextureWrappingMode wrapping_mode, int8_t dimension = -1);

protected:
    RaTexture(RaContext const& context) : RaContractWithRaContext{ context } {}

protected:
    void initialize(uint32_t width, uint32_t height, RaTextureFilteringMode minification_filter, RaTextureFilteringMode magnification_filter,
        RaTextureFilteringMode mipmapping_filter, RaTextureWrappingMode wrapping_mode, float anisotropy, bool srgb_texture);

    void attachBuffer(RaAbstractBuffer const& buffer);

protected:
    std::shared_ptr<RTtexturesampler_api> m_sampler;
};

template<typename T>
class RaTypedTexture : public RaTexture
{
public:
    RaAbstractBuffer& buffer() override { return *m_buffer; }

    int id() const 
    { 
        int rv{};
        rtTextureSamplerGetId(*m_sampler, &rv); 
        return rv;
    }

protected:
    RaTypedTexture(RaContext const& context) : RaTexture{ context } {}

    void initialize(uint32_t width, uint32_t height, RaTextureFilteringMode minification_filter, RaTextureFilteringMode magnification_filter,
        RaTextureFilteringMode mipmapping_filter, RaTextureWrappingMode wrapping_mode, float anisotropy, bool srgb_texture)
    {
        RaTexture::initialize(width, height, minification_filter, magnification_filter, mipmapping_filter, wrapping_mode, anisotropy, srgb_texture);

        m_buffer.reset(new RaBuffer<T>{ context().createBuffer<T>(RaBufferKind::input, width, height) });
        attachBuffer(*m_buffer);
    }

private:
    std::unique_ptr<RaBuffer<T>> m_buffer;
};
}

#endif
