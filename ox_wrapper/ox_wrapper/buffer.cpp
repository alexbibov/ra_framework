#include "buffer.h"

using namespace ox_wrapper;

OxBufferId OxAbstractBuffer::getId() const
{
    OxBufferId rv;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetId(m_native_optix_buffer.get(), &rv.native));
    return rv;
}

void* OxAbstractBuffer::map(OxBufferMapKind map_kind, unsigned int mipmap_level) const
{
    void* rv{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferMapEx(nativeOptiXBufferHandle(), static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
    return rv;
}

void OxAbstractBuffer::unmap(unsigned int mipmap_level) const
{
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferUnmapEx(m_native_optix_buffer.get(), mipmap_level));
}

OxBufferDimension OxAbstractBuffer::getDimension() const
{
    unsigned int dim{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetDimensionality(m_native_optix_buffer.get(), &dim));
    return static_cast<OxBufferDimension>(dim);
}

void OxAbstractBuffer::getSize(size_t* width, size_t* height, size_t* depth) const
{
    OxBufferDimension dim = getDimension();

    RTsize w{}, h{}, d{};

    switch (dim)
    {
    case OxBufferDimension::_1D:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize1D(m_native_optix_buffer.get(), &w));
        h = 1U;
        d = 1U;
        break;
    case OxBufferDimension::_2D:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize2D(m_native_optix_buffer.get(), &w, &h));
        d = 1U;
        break;
    case OxBufferDimension::_3D:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize3D(m_native_optix_buffer.get(), &w, &h, &d));
        break;
    default:
        w = h = d = static_cast<RTsize>(-1);
        break;
    }

    *width = w; *height = h; *depth = d;
}

size_t OxAbstractBuffer::getWidth() const
{
    size_t w{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize1D(m_native_optix_buffer.get(), &w));
    return static_cast<size_t>(w);
}

size_t OxAbstractBuffer::getHeight() const
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

size_t OxAbstractBuffer::getDepth() const
{
    RTsize w{}, h{}, d{};
    if (getDimension() == OxBufferDimension::_3D)
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetSize3D(m_native_optix_buffer.get(), &w, &h, &d));
    }
    else
    {
        d = 1U;
    }

    return static_cast<size_t>(d);
}

size_t OxAbstractBuffer::getElementSize() const
{
    RTsize element_size{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferGetElementSize(m_native_optix_buffer.get(), &element_size));

    return static_cast<size_t>(element_size);
}

size_t OxAbstractBuffer::getCapacityInBytes() const
{
    return getWidth()*getHeight()*getDepth()*getElementSize();
}

bool OxAbstractBuffer::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtBufferValidate(m_native_optix_buffer.get()));
    return res == RT_SUCCESS;
}

OxAbstractBuffer::OxAbstractBuffer(OxContext const& context):
    OxContractWithOxContext{ context }
{
}

RTbuffer OxAbstractBuffer::nativeOptiXBufferHandle() const
{
    return m_native_optix_buffer.get();
}

void OxAbstractBuffer::assignNativeOptixBufferHandle(RTbuffer native_optix_buffer_handle)
{
    m_native_optix_buffer.reset(
        native_optix_buffer_handle,
        [this](RTbuffer b) -> void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferDestroy(b));
    });
}

OxBufferMapSentry<void> ox_wrapper::makeBufferMapSentry(OxAbstractBuffer const& buffer, OxBufferMapKind map_kind, unsigned int mipmap_level)
{
    return OxBufferMapSentry<void>{ buffer, map_kind, mipmap_level };
}
