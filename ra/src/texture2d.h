#ifndef RA_TEXTURE2D_H
#define RA_TEXTURE2D_H

#include <string>

#include "stb/stb_image.h"
#include "entity.h"
#include "texture.h"

namespace ra {

template<typename T>
class RaTexture2D : public RaTexture<T>
{
public:
    RaTexture2D(RaContext const& context, std::string const& filename,
        RaTextureFilteringMode filtering_mode = RaTextureFilteringMode::linear,
        RaTextureWrappingMode wrapping_mode = RaTextureWrappingMode::clamp_to_edge);

    RaTexture2D(RaContext const& context, uint32_t width, uint32_t height, RaTexturePixelFormat pixel_format,
        RaTextureFilteringMode filtering_mode = RaTextureFilteringMode::linear,
        RaTextureWrappingMode wrapping_mode = RaTextureWrappingMode::clamp_to_edge);

    RaAbstractBuffer& buffer();

    int id() const;

};

}


#endif
