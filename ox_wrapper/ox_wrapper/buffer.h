#ifndef OX_WRAPPER_BUFFER_H
#define OX_WRAPPER_BUFFER_H

#include <memory>

#include "optix.h"
#include "vector_types.h"

#include "ox_wrapper/util/log.h"
#include "ox_wrapper/ox_wrapper_fwd.h"
#include "ox_wrapper/contract_with_context.h"
#include "ox_wrapper/entity.h"
#include "ox_wrapper/util/optional.h"


namespace ox_wrapper {

enum class OxBufferKind
{
    input = RT_BUFFER_INPUT,
    output = RT_BUFFER_OUTPUT,
    input_output = RT_BUFFER_INPUT_OUTPUT
};

enum class OxBufferMapKind
{
    read = RT_BUFFER_MAP_READ,
    write = RT_BUFFER_MAP_WRITE,
    read_write = RT_BUFFER_MAP_READ_WRITE,
    write_discard = RT_BUFFER_MAP_WRITE_DISCARD
};

enum class OxBufferDimension : unsigned char
{
    _1D = 1, _2D = 2, _3D = 3
};

struct OxBufferId
{
    int native;
};


enum class OxBasicBufferFormat
{
    FLOAT, FLOAT2, FLOAT3, FLOAT4,
    INT, INT2, INT3, INT4,
    UINT, UINT2, UINT3, UINT4,
    CHAR, CHAR2, CHAR3, CHAR4,
    UCHAR, UCHAR2, UCHAR3, UCHAR4,
    RAY_RADIANCE_PAYLOAD, RAY_RADIANCE_PAYLOAD_SIMPLE,
    RAY_RADIANCE_PAYLOAD_MONOCHROMATIC,
    RAY_OCCLUSION_PAYLOAD
};


template<typename T> class OxAbstractBufferAttorney;

class OxAbstractBuffer : public OxContractWithOxContext, public OxEntity
{
    friend class OxAbstractBufferAttorney<OxProgram>;

public:
    virtual ~OxAbstractBuffer() = default;

    OxBufferId getId() const;

    void* map(OxBufferMapKind map_kind, unsigned int mipmap_level = 0U) const;
    void unmap(unsigned int mipmap_level = 0U) const;

    OxBufferDimension getDimension() const;
    void getSize(size_t* width, size_t* height, size_t* depth) const;
    size_t getWidth() const;
    size_t getHeight() const;
    size_t getDepth() const;
    size_t getElementSize() const;
    size_t getCapacityInBytes() const;

    // required by OxEntity interface
    bool isValid() const override;

protected:
    OxAbstractBuffer(OxContext const& context);

    RTbuffer nativeOptiXBufferHandle() const;
    void assignNativeOptixBufferHandle(RTbuffer native_optix_buffer_handle);

private:
    std::shared_ptr<RTbuffer_api> m_native_optix_buffer;
};

template<> class OxAbstractBufferAttorney<OxProgram>
{
    friend class OxProgram;

    static RTbuffer getNativeOptiXBufferHandle(OxAbstractBuffer const& parent_abstract_buffer)
    {
        return parent_abstract_buffer.nativeOptiXBufferHandle();
    }
};


template<typename T> class OxBufferAttorney;

template<typename T>
class OxBuffer final : public OxAbstractBuffer
{
    friend class OxBufferAttorney<OxContext>;

public:
    using value_type = T;

public:
    ~OxBuffer() = default;

private:
#include "buffer_tuner.inl"
    using my_tuner_type = OxBufferTuner<T>;


    OxBuffer(OxContext const& context,
        OxBufferKind buffer_kind, size_t width) :
        OxAbstractBuffer{ context }
    {
        assignNativeOptixBufferHandle(my_tuner_type::create_buffer(*this, buffer_kind));
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferSetSize1D(nativeOptiXBufferHandle(), width));
    }

    OxBuffer(OxContext const& context,
        OxBufferKind buffer_kind, size_t width, size_t height) :
        OxAbstractBuffer{ context }
    {
        assignNativeOptixBufferHandle(my_tuner_type::create_buffer(*this, buffer_kind));
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferSetSize2D(nativeOptiXBufferHandle(), width, height));
    }

    OxBuffer(OxContext const& context,
        OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) :
        OxAbstractBuffer{ context }
    {
        assignNativeOptixBufferHandle(my_tuner_type::create_buffer(*this, buffer_kind));
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferSetSize3D(nativeOptiXBufferHandle(), width, height, depth));
    }

public:
    T * map(OxBufferMapKind map_kind, unsigned int mipmap_level = 0U) const
    {
        void* rv{ nullptr };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferMapEx(nativeOptiXBufferHandle(), static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
        return static_cast<T*>(rv);
    }
};

template<> class OxBufferAttorney<OxContext>
{
    friend class OxContext;

    template<typename T>
    static OxBuffer<T> createOptiXBuffer(OxContext const& optix_context, OxBufferKind buffer_kind, size_t width)
    {
        return OxBuffer<T>{ optix_context, buffer_kind, width };
    }

    template<typename T>
    static OxBuffer<T> createOptiXBuffer(OxContext const& optix_context, OxBufferKind buffer_kind, size_t width, size_t height)
    {
        return OxBuffer<T>{ optix_context, buffer_kind, width, height };
    }

    template<typename T>
    static OxBuffer<T> createOptiXBuffer(OxContext const& optix_context, OxBufferKind buffer_kind, size_t width, size_t height, size_t depth)
    {
        return OxBuffer<T>{ optix_context, buffer_kind, width, height, depth };
    }
};

template<typename T>
util::Optional<OxBuffer<T>> castBufferToType(OxAbstractBuffer& source_buffer)
{
    OxBuffer<T>* p_dest_buffer = static_cast<OxBuffer<T>*>(&source_buffer);
    if (p_dest_buffer)
        return util::Optional<OxBuffer<T>>{*p_dest_buffer};

    return util::Optional<OxBuffer<T>>{};
}

template<typename T>
util::Optional<OxBuffer<T> const> castBufferToType(OxAbstractBuffer const& source_buffer)
{
    return castBufferToType<T>(const_cast<OxAbstractBuffer&>(source_buffer));
}


template<typename T> class OxBufferMapSentry;

template<typename T>
class OxBufferMapSentry final
{
public:
    OxBufferMapSentry(OxBuffer<T> const& buffer_to_map,
        OxBufferMapKind map_kind, unsigned int mipmap_level = 0U) :
        m_mapped_buffer{ buffer_to_map },
        m_mapped_mipmap_level{ mipmap_level },
        m_addr{ m_mapped_buffer.map(map_kind, mipmap_level) }
    {

    }

    OxBufferMapSentry(OxBufferMapSentry const&) = delete;
    OxBufferMapSentry(OxBufferMapSentry&&) = default;

    OxBufferMapSentry& operator=(OxBufferMapSentry const&) = delete;
    OxBufferMapSentry& operator=(OxBufferMapSentry&&) = delete;

    T* address() const { return m_addr; }

    ~OxBufferMapSentry()
    {
        m_mapped_buffer.unmap(m_mapped_mipmap_level);
    }

private:
    OxBuffer<T> const& m_mapped_buffer;
    unsigned int m_mapped_mipmap_level;
    T* m_addr;
};


template<>
class OxBufferMapSentry<void> final
{
public:
    OxBufferMapSentry(OxAbstractBuffer const& buffer_to_map,
        OxBufferMapKind map_kind, unsigned int mipmap_level = 0U) :
        m_mapped_buffer{ buffer_to_map },
        m_mapped_mipmap_level{ mipmap_level },
        m_addr{ m_mapped_buffer.map(map_kind, mipmap_level) }
    {
    }

    OxBufferMapSentry(OxBufferMapSentry const&) = delete;
    OxBufferMapSentry(OxBufferMapSentry&&) = default;

    OxBufferMapSentry& operator=(OxBufferMapSentry const&) = delete;
    OxBufferMapSentry& operator=(OxBufferMapSentry&&) = delete;

    void* address() const { return m_addr; }

    ~OxBufferMapSentry()
    {
        m_mapped_buffer.unmap(m_mapped_mipmap_level);
    }

private:
    OxAbstractBuffer const& m_mapped_buffer;
    unsigned int m_mapped_mipmap_level;
    void* m_addr;
};


template<typename T>
OxBufferMapSentry<T> makeBufferMapSentry(OxBuffer<T> const& buffer,
    OxBufferMapKind map_kind, unsigned int mipmap_level = 0U)
{
    return OxBufferMapSentry<T>{ buffer, map_kind, mipmap_level };
}

OxBufferMapSentry<void> makeBufferMapSentry(OxAbstractBuffer const& buffer,
    OxBufferMapKind map_kind, unsigned int mipmap_level = 0U);

}


#endif
