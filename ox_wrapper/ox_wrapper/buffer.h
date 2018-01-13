#ifndef OX_WRAPPER_BUFFER_H
#define OX_WRAPPER_BUFFER_H

#include <memory>

#include "optix.h"
#include "util/log.h"
#include "../../CUDA/v9.0/include/vector_types.h"
#include "fwd.h"
#include "contract_with_context.h"
#include "entity.h"
#include "util/optional.h"


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
    T* map(OxBufferMapKind map_kind, unsigned int mipmap_level = 0U) const
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
    OxBuffer<T>* p_dest_buffer = dynamic_cast<OxBuffer<T>*>(&source_buffer);
    if (p_dest_buffer)
        return util::Optional<OxBuffer<T>>{*p_dest_buffer};

    return util::Optional<OxBuffer<T>>{};
}

template<typename T>
util::Optional<OxBuffer<T> const> castBufferToType(OxAbstractBuffer const& source_buffer)
{
    return castBufferToType<T>(const_cast<OxAbstractBuffer&>(source_buffer));
}

}

#endif
