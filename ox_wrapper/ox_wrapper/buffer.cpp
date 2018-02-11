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
    rtBufferGetDimensionality(m_native_optix_buffer.get(), &dim);
    return static_cast<OxBufferDimension>(dim);
}

void OxAbstractBuffer::getSize(size_t* width, size_t* height, size_t* depth) const
{
    OxBufferDimension dim = getDimension();
    switch (dim)
    {
    case OxBufferDimension::_1D:
        rtBufferGetSize1D(m_native_optix_buffer.get(), width);
        *height = 1U;
        *depth = 1U;
        break;
    case OxBufferDimension::_2D:
        rtBufferGetSize2D(m_native_optix_buffer.get(), width, height);
        *depth = 1U;
        break;
    case OxBufferDimension::_3D:
        rtBufferGetSize3D(m_native_optix_buffer.get(), width, height, depth);
        break;
    default:
        *width = *height = *depth = static_cast<unsigned int>(-1);
        break;
    }
}

size_t OxAbstractBuffer::getWidth() const
{
    size_t w{};
    rtBufferGetSize1D(m_native_optix_buffer.get(), &w);
    return w;
}

size_t OxAbstractBuffer::getHeight() const
{
    size_t w{}, h{};
    if (static_cast<unsigned char>(getDimension()) >= 2)
    {
        rtBufferGetSize2D(m_native_optix_buffer.get(), &w, &h);
    }
    else
    {
        h = 1U;
    }

    return h;
}

size_t OxAbstractBuffer::getDepth() const
{
    RTsize w{}, h{}, d{};
    if (getDimension() == OxBufferDimension::_3D)
    {
        rtBufferGetSize3D(m_native_optix_buffer.get(), &w, &h, &d);
    }
    else
    {
        d = 1U;
    }

    return d;
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

OxBufferMapSentinel::OxBufferMapSentinel(
    OxAbstractBuffer const& buffer_to_map,
    OxBufferMapKind map_kind, 
    unsigned int mipmap_level/* = 0U*/) :
    m_mapped_buffer{ buffer_to_map },
    m_counter{ new size_t{ 1U } },
    m_data_ptr{ m_mapped_buffer.map(map_kind, mipmap_level) },
    m_mapped_mipmap_level{ mipmap_level }
{
}

OxBufferMapSentinel::OxBufferMapSentinel(OxBufferMapSentinel const& other):
    m_mapped_buffer{ other.m_mapped_buffer },
    m_counter{ other.m_counter },
    m_data_ptr{ other.m_data_ptr },
    m_mapped_mipmap_level{ other.m_mapped_mipmap_level }
{
    ++(*m_counter);
}

OxBufferMapSentinel::OxBufferMapSentinel(OxBufferMapSentinel&& other):
    m_mapped_buffer{ other.m_mapped_buffer },
    m_counter{ other.m_counter },
    m_data_ptr{ other.m_counter },
    m_mapped_mipmap_level{ other.m_mapped_mipmap_level }
{
    other.m_counter = nullptr;
}

ox_wrapper::OxBufferMapSentinel::~OxBufferMapSentinel()
{
    if (m_counter)
    {
        --(*m_counter);
        if (!(*m_counter))
        {
            delete m_counter;
            m_mapped_buffer.unmap(m_mapped_mipmap_level);
        }
    }
}

void* OxBufferMapSentinel::data() const
{
    return m_data_ptr;
}


