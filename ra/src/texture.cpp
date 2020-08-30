#include "texture.h"

using namespace ra;

bool RaTexture::isValid() const
{
    return rtTextureSamplerValidate(m_sampler.get()) == RT_SUCCESS;
}

void RaTexture::setFilteringMode(RaTextureFilteringMode minification_filter,
    RaTextureFilteringMode magnification_filter, RaTextureFilteringMode mipmapping_filter, float anisotropy)
{
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetFilteringModes(m_sampler.get(),
        static_cast<RTfiltermode>(minification_filter), static_cast<RTfiltermode>(magnification_filter), static_cast<RTfiltermode>(mipmapping_filter)));

    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetMaxAnisotropy(m_sampler.get(), anisotropy));
}

void RaTexture::setWrappingMode(RaTextureWrappingMode wrapping_mode, int8_t dimension)
{
    if (dimension > 0)
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetWrapMode(m_sampler.get(), dimension, static_cast<RTwrapmode>(wrapping_mode)));
    }
    else
    {
        RaBufferDimension dim = buffer().getDimension();
        for (int i = 0; i < static_cast<int>(dim); ++i)
        {
            LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetWrapMode(m_sampler.get(), static_cast<unsigned int>(dim), static_cast<RTwrapmode>(wrapping_mode)));
        }
    }
}

void RaTexture::initialize(uint32_t width, uint32_t height, RaTextureFilteringMode minification_filter, RaTextureFilteringMode magnification_filter, RaTextureFilteringMode mipmapping_filter, RaTextureWrappingMode wrapping_mode, float anisotropy, bool srgb_texture)
{
    {
        RTtexturesampler native_texture_sampler{};
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerCreate(nativeOptiXContextHandle(), &native_texture_sampler));
        m_sampler.reset(native_texture_sampler,
            [this](RTtexturesampler s) {LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerDestroy(s)); });

        setFilteringMode(minification_filter, magnification_filter, mipmapping_filter, anisotropy);
        setWrappingMode(wrapping_mode);
       
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetIndexingMode(native_texture_sampler, RT_TEXTURE_INDEX_NORMALIZED_COORDINATES));
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetReadMode(native_texture_sampler, srgb_texture ? RT_TEXTURE_READ_NORMALIZED_FLOAT_SRGB : RT_TEXTURE_READ_NORMALIZED_FLOAT));
    }
}

void RaTexture::attachBuffer(RaAbstractBuffer const& buffer)
{
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTextureSamplerSetBuffer(m_sampler.get(), 0, 0, RaAbstractBufferAttorney<RaTexture>::getNativeOptiXBufferHandle(buffer)));
}
