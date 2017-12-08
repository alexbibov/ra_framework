#ifndef OX_WRAPPER_BUFFER_H
#define OX_WRAPPER_BUFFER_H

#include <memory>

#include "optix.h"
#include "util/log.h"
#include "../../CUDA/v9.0/include/vector_types.h"
#include "fwd.h"
#include "contract_with_context.h"
#include "entity.h"


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

struct OxBufferId
{
    int native;
};

template<typename T> class OxBufferAttorney;

template<typename T>
class OxBuffer final : public OxContractWithOxContext, public OxEntity
{
    friend class OxBufferAttorney<OxContext>;
    friend class OxBufferAttorney<OxProgram>;

public:

    ~OxBuffer() = default;

    OxBufferId getId() const
    {
        OxBufferId rv;
        throwOptiXContextError(rtBufferGetId(m_native_optix_buffer.get(), &rv.native));
        return rv;
    }

private:
    #include "buffer_tuner.inl"
    using my_tuner_type = OxBufferTuner<T>;


    OxBuffer(OxContext const& optix_context,
        OxBufferKind buffer_kind, size_t width) :
        OxContractWithOxContext{ optix_context }
    {
        m_native_optix_buffer.reset(my_tuner_type::create_buffer(*this, buffer_kind),
            [this](RTbuffer b) -> void
        {
            logOptiXContextError(rtBufferDestroy(b));
        });
        throwOptiXContextError(rtBufferSetSize1D(m_native_optix_buffer.get(), width));
    }

    OxBuffer(OxContext const& optix_context,
        OxBufferKind buffer_kind, size_t width, size_t height) :
        OxContractWithOxContext{ optix_context }
    {
        m_native_optix_buffer.reset(my_tuner_type::create_buffer(*this, buffer_kind),
            [this](RTbuffer b) -> void
        {
            logOptiXContextError(rtBufferDestroy(b));
        });
        throwOptiXContextError(rtBufferSetSize2D(m_native_optix_buffer.get(), width, height));
    }

    OxBuffer(OxContext const& optix_context,
        OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) :
        OxContractWithOxContext{ optix_context }
    {
        m_native_optix_buffer.reset(my_tuner_type::create_buffer(*this, buffer_kind),
            [this](RTbuffer b) -> void
        {
            logOptiXContextError(rtBufferDestroy(b));
        });
        throwOptiXContextError(rtBufferSetSize3D(m_native_optix_buffer.get(), width, height, depth));
    }

    // required by OxEntity interface
    bool isValid() const override
    {
        RTresult res = rtBufferValidate(m_native_optix_buffer.get());
        logOptiXContextError(res);
        return res == RT_SUCCESS;
    }

public:
    void* map(OxBufferMapKind map_kind, unsigned int mipmap_level = 0U) const
    {
        void* rv{ nullptr };
        throwOptiXContextError(rtBufferMapEx(m_native_optix_buffer.get(), static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
        return rv;
    }

    void unmap(unsigned int mipmap_level = 0U) const
    {
        throwOptiXContextError(rtBufferUnmapEx(m_native_optix_buffer.get(), mipmap_level));
    }

private:
    std::shared_ptr<RTbuffer_api> m_native_optix_buffer;
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

template<> class OxBufferAttorney<OxProgram>
{
    friend class OxProgram;

    template<typename T>
    static RTbuffer getNativeOptiXBufferHandle(OxBuffer<T> const& parent_buffer)
    {
        return parent_buffer.m_native_optix_buffer.get();
    }
};


}

#endif
