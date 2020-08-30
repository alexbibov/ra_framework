#ifndef RA_TEXTURE2D_H
#define RA_TEXTURE2D_H

#include <string>

#include "entity.h"
#include "texture.h"

namespace ra {

template<typename T>
class RaTexture2D : public RaTypedTexture<T>
{
public:
    RaTexture2D(RaContext const& context, uint32_t width, uint32_t height,
        RaTextureFilteringMode minification_filter = RaTextureFilteringMode::linear,
        RaTextureFilteringMode magnification_filter = RaTextureFilteringMode::linear,
        RaTextureFilteringMode mipmapping_filter = RaTextureFilteringMode::nearest,
        RaTextureWrappingMode wrapping_mode = RaTextureWrappingMode::clamp_to_edge,
        float anisotropy = 1.f, bool srgb_texture = false)
        : RaTypedTexture{ context } 
    {
        initialize(width, height, minification_filter, magnification_filter, mipmapping_filter, wrapping_mode, anisotropy, srgb_texture);
    }
};


}


#endif
