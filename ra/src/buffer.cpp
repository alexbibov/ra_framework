#include "buffer.h"

using namespace ra;

RaBufferId RaAbstractBuffer::getId() const
{
    RaBufferId rv;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetId(m_native_optix_buffer.get(), &rv.native));
    return rv;
}

void* RaAbstractBuffer::map(RaBufferMapKind map_kind, unsigned int mipmap_level) const
{
    void* rv{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferMapEx(nativeOptiXBufferHandle(), static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
    return rv;
}

void RaAbstractBuffer::unmap(unsigned int mipmap_level) const
{
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferUnmapEx(m_native_optix_buffer.get(), mipmap_level));
}

RaBufferDimension RaAbstractBuffer::getDimension() const
{
    unsigned int dim{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetDimensionality(m_native_optix_buffer.get(), &dim));
    return static_cast<RaBufferDimension>(dim);
}

void RaAbstractBuffer::getSize(size_t* width, size_t* height, size_t* depth) const
{
    RaBufferDimension dim = getDimension();

    RTsize w{}, h{}, d{};

    switch (dim)
    {
    case RaBufferDimension::_1D:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize1D(m_native_optix_buffer.get(), &w));
        h = 1U;
        d = 1U;
        break;
    case RaBufferDimension::_2D:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize2D(m_native_optix_buffer.get(), &w, &h));
        d = 1U;
        break;
    case RaBufferDimension::_3D:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize3D(m_native_optix_buffer.get(), &w, &h, &d));
        break;
    default:
        w = h = d = static_cast<RTsize>(-1);
        break;
    }

    *width = w; *height = h; *depth = d;
}

size_t RaAbstractBuffer::getWidth() const
{
    size_t w{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize1D(m_native_optix_buffer.get(), &w));
    return static_cast<size_t>(w);
}

size_t RaAbstractBuffer::getHeight() const
{
    size_t w{}, h{};
    if (static_cast<unsigned char>(getDimension()) >= 2)
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize2D(m_native_optix_buffer.get(), &w, &h));
    }
    else
    {
        h = 1U;
    }

    return static_cast<size_t>(h);
}

size_t RaAbstractBuffer::getDepth() const
{
    RTsize w{}, h{}, d{};
    if (getDimension() == RaBufferDimension::_3D)
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize3D(m_native_optix_buffer.get(), &w, &h, &d));
    }
    else
    {
        d = 1U;
    }

    return static_cast<size_t>(d);
}

size_t RaAbstractBuffer::getElementSize() const
{
    RTsize element_size{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetElementSize(m_native_optix_buffer.get(), &element_size));

    return static_cast<size_t>(element_size);
}

size_t RaAbstractBuffer::getCapacityInBytes() const
{
    return getWidth()*getHeight()*getDepth()*getElementSize();
}

bool RaAbstractBuffer::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtBufferValidate(m_native_optix_buffer.get()));
    return res == RT_SUCCESS;
}

RaAbstractBuffer::RaAbstractBuffer(RaContext const& context):
    RaContractWithRaContext{ context }
{
}

RTbuffer RaAbstractBuffer::nativeOptiXBufferHandle() const
{
    return m_native_optix_buffer.get();
}

void RaAbstractBuffer::assignNativeOptixBufferHandle(RTbuffer native_optix_buffer_handle)
{
    m_native_optix_buffer.reset(
        native_optix_buffer_handle,
        [this](RTbuffer b) -> void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferDestroy(b));
    });
}

RaBufferMapSentry<void> ra::makeBufferMapSentry(RaAbstractBuffer const& buffer, RaBufferMapKind map_kind, unsigned int mipmap_level)
{
    return RaBufferMapSentry<void>{ buffer, map_kind, mipmap_level };
}
