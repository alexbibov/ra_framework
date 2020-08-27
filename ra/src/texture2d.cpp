#include "texture2d.h"
#include <filesystem>

using namespace ra;


namespace {
    uint8_t getPixelWidth(RaTexturePixelFormat pixel_format)
    {
        assert(static_cast<uint8_t>(RaTexturePixelFormat::r) == 0);
        return (static_cast<uint8_t>(pixel_format) + 1) * sizeof(float);
    }
}

RaTexture2D::RaTexture2D(RaContext const& context, std::string const& filename, RaTextureFilteringMode filtering_mode, RaTextureWrappingMode wrapping_mode)
    : RaContractWithRaContext{ context }
{
}

RaTexture2D::RaTexture2D(RaContext const& context, uint32_t width, uint32_t height, RaTexturePixelFormat pixel_format, RaTextureFilteringMode filtering_mode, RaTextureWrappingMode wrapping_mode)
    : RaContractWithRaContext{ context }
{
    initialize(width, height, pixel_format, filtering_mode, wrapping_mode);
}

void RaTexture2D::initialize(uint32_t width, uint32_t height, RaTexturePixelFormat pixel_format, RaTextureFilteringMode filtering_mode, RaTextureWrappingMode wrapping_mode)
{
    m_buffer.reset(new RaBuffer{ context(), RaBufferKind::input, width, height })
}
