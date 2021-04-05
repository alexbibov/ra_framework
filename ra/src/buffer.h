#ifndef RA_BUFFER_H
#define RA_BUFFER_H

#include <memory>

#include "optix.h"
#include "vector_types.h"

#include "util/log.h"
#include "util/optix_format_traits.h"
#include "ra_fwd.h"
#include "contract_with_context.h"
#include "entity.h"
#include "util/optional.h"


namespace ra {

enum class RaBufferKind
{
    input = RT_BUFFER_INPUT,
    output = RT_BUFFER_OUTPUT,
    input_output = RT_BUFFER_INPUT_OUTPUT
};

enum class RaBufferMapKind
{
    read = RT_BUFFER_MAP_READ,
    write = RT_BUFFER_MAP_WRITE,
    read_write = RT_BUFFER_MAP_READ_WRITE,
    write_discard = RT_BUFFER_MAP_WRITE_DISCARD
};

enum class RaBufferDimension : unsigned char
{
    _1D = 1, _2D = 2, _3D = 3
};

struct RaBufferId
{
    int native;
};


enum class RaBasicBufferFormat
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


template<typename T> class RaAbstractBufferAttorney;

class RaAbstractBuffer : public RaContractWithRaContext, public RaEntity
{
    friend class RaAbstractBufferAttorney<RaProgram>;
    friend class RaAbstractBufferAttorney<RaTexture>;

public:
    virtual ~RaAbstractBuffer() = default;

    RaBufferId getId() const;

    void* map(RaBufferMapKind map_kind, unsigned int mipmap_level = 0U) const;
    void unmap(unsigned int mipmap_level = 0U) const;

    RaBufferDimension getDimension() const;
    void getSize(size_t* width, size_t* height, size_t* depth) const;
    size_t getWidth() const;
    size_t getHeight() const;
    size_t getDepth() const;
    size_t getElementSize() const;
    size_t getCapacityInBytes() const;

    // required by RaEntity interface
    bool isValid() const override;

protected:
    RaAbstractBuffer(RaContext const& context);

    RTbuffer nativeOptiXBufferHandle() const;
    void assignNativeOptixBufferHandle(RTbuffer native_optix_buffer_handle);

private:
    std::shared_ptr<RTbuffer_api> m_native_optix_buffer;
};

template<> class RaAbstractBufferAttorney<RaProgram>
{
    friend class RaProgram;

    static RTbuffer getNativeOptiXBufferHandle(RaAbstractBuffer const& parent_abstract_buffer)
    {
        return parent_abstract_buffer.nativeOptiXBufferHandle();
    }
};


template<> class RaAbstractBufferAttorney<RaTexture>
{
    friend class RaTexture;

    static RTbuffer getNativeOptiXBufferHandle(RaAbstractBuffer const& parent_buffer)
    {
        return parent_buffer.m_native_optix_buffer.get();
    }
};


template<typename T> class RaBufferAttorney;

template<typename T>
class RaBuffer final : public RaAbstractBuffer
{
    friend class RaBufferAttorney<RaContext>;

public:
    using value_type = T;

public:
    ~RaBuffer() = default;

private:
#include "buffer_tuner.inl"
    using my_tuner_type = RaBufferTuner<T>;


    RaBuffer(RaContext const& context,
        RaBufferKind buffer_kind, size_t width) :
        RaAbstractBuffer{ context }
    {
        assignNativeOptixBufferHandle(my_tuner_type::create_buffer(*this, buffer_kind));
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferSetSize1D(nativeOptiXBufferHandle(), width));
    }

    RaBuffer(RaContext const& context,
        RaBufferKind buffer_kind, size_t width, size_t height) :
        RaAbstractBuffer{ context }
    {
        assignNativeOptixBufferHandle(my_tuner_type::create_buffer(*this, buffer_kind));
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferSetSize2D(nativeOptiXBufferHandle(), width, height));
    }

    RaBuffer(RaContext const& context,
        RaBufferKind buffer_kind, size_t width, size_t height, size_t depth) :
        RaAbstractBuffer{ context }
    {
        assignNativeOptixBufferHandle(my_tuner_type::create_buffer(*this, buffer_kind));
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferSetSize3D(nativeOptiXBufferHandle(), width, height, depth));
    }

public:
    T* map(RaBufferMapKind map_kind, unsigned int mipmap_level = 0U) const
    {
        void* rv{ nullptr };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtBufferMapEx(nativeOptiXBufferHandle(), static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
        return static_cast<T*>(rv);
    }
};

template<> class RaBufferAttorney<RaContext>
{
    friend class RaContext;

    template<typename T>
    static RaBuffer<T> createOptiXBuffer(RaContext const& optix_context, RaBufferKind buffer_kind, size_t width)
    {
        return RaBuffer<T>{ optix_context, buffer_kind, width };
    }

    template<typename T>
    static RaBuffer<T> createOptiXBuffer(RaContext const& optix_context, RaBufferKind buffer_kind, size_t width, size_t height)
    {
        return RaBuffer<T>{ optix_context, buffer_kind, width, height };
    }

    template<typename T>
    static RaBuffer<T> createOptiXBuffer(RaContext const& optix_context, RaBufferKind buffer_kind, size_t width, size_t height, size_t depth)
    {
        return RaBuffer<T>{ optix_context, buffer_kind, width, height, depth };
    }
};

template<typename T>
util::Optional<RaBuffer<T>> castBufferToType(RaAbstractBuffer& source_buffer)
{
    RaBuffer<T>* p_dest_buffer = dynamic_cast<RaBuffer<T>*>(&source_buffer);
    if (p_dest_buffer)
        return util::Optional<RaBuffer<T>>{*p_dest_buffer};

    return util::Optional<RaBuffer<T>>{};
}

template<typename T>
util::Optional<RaBuffer<T> const> castBufferToType(RaAbstractBuffer const& source_buffer)
{
    return castBufferToType<T>(const_cast<RaAbstractBuffer&>(source_buffer));
}


template<typename T> class RaBufferMapSentry;

template<typename T>
class RaBufferMapSentry final
{
public:
    RaBufferMapSentry(RaBuffer<T> const& buffer_to_map,
        RaBufferMapKind map_kind, unsigned int mipmap_level = 0U) :
        m_mapped_buffer{ buffer_to_map },
        m_mapped_mipmap_level{ mipmap_level },
        m_addr{ m_mapped_buffer.map(map_kind, mipmap_level) }
    {

    }

    RaBufferMapSentry(RaBufferMapSentry const&) = delete;
    RaBufferMapSentry(RaBufferMapSentry&&) = default;

    RaBufferMapSentry& operator=(RaBufferMapSentry const&) = delete;
    RaBufferMapSentry& operator=(RaBufferMapSentry&&) = delete;

    T* address() const { return m_addr; }

    ~RaBufferMapSentry()
    {
        m_mapped_buffer.unmap(m_mapped_mipmap_level);
    }

private:
    RaBuffer<T> const& m_mapped_buffer;
    unsigned int m_mapped_mipmap_level;
    T* m_addr;
};


template<>
class RaBufferMapSentry<void> final
{
public:
    RaBufferMapSentry(RaAbstractBuffer const& buffer_to_map,
        RaBufferMapKind map_kind, unsigned int mipmap_level = 0U) :
        m_mapped_buffer{ buffer_to_map },
        m_mapped_mipmap_level{ mipmap_level },
        m_addr{ m_mapped_buffer.map(map_kind, mipmap_level) }
    {
    }

    RaBufferMapSentry(RaBufferMapSentry const&) = delete;
    RaBufferMapSentry(RaBufferMapSentry&&) = default;

    RaBufferMapSentry& operator=(RaBufferMapSentry const&) = delete;
    RaBufferMapSentry& operator=(RaBufferMapSentry&&) = delete;

    void* address() const { return m_addr; }

    ~RaBufferMapSentry()
    {
        m_mapped_buffer.unmap(m_mapped_mipmap_level);
    }

private:
    RaAbstractBuffer const& m_mapped_buffer;
    unsigned int m_mapped_mipmap_level;
    void* m_addr;
};


template<typename T>
RaBufferMapSentry<T> makeBufferMapSentry(RaBuffer<T> const& buffer,
    RaBufferMapKind map_kind, unsigned int mipmap_level = 0U)
{
    return RaBufferMapSentry<T>{ buffer, map_kind, mipmap_level };
}

RaBufferMapSentry<void> makeBufferMapSentry(RaAbstractBuffer const& buffer,
    RaBufferMapKind map_kind, unsigned int mipmap_level = 0U);

}


#endif
