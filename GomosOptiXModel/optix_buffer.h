#ifndef OX_WRAPPER_OPTIX_BUFFER_H
#define OX_WRAPPER_OPTIX_BUFFER_H

#include <memory>

#include "optix.h"
#include "log.h"
#include "../CUDA/v9.0/include/vector_types.h"
#include "ox_wrapper_fwd.h"
#include "has_contract_with_optix_context.h"


namespace ox_wrapper {

enum class OptiXBufferKind
{
    input = RT_BUFFER_INPUT,
    output = RT_BUFFER_OUTPUT,
    input_output = RT_BUFFER_INPUT_OUTPUT
};

enum class OptiXBufferMapKind
{
    read = RT_BUFFER_MAP_READ,
    write = RT_BUFFER_MAP_WRITE,
    read_write = RT_BUFFER_MAP_READ_WRITE,
    write_discard = RT_BUFFER_MAP_WRITE_DISCARD
};

struct bufferId
{
    int native;
};

template<typename T> class OptiXBufferAttorney;

template<typename T>
class OptiXBuffer final : public HasContractWithOptiXContext
{
    friend class OptiXBufferAttorney<OptiXContext>;

public:

    ~OptiXBuffer() = default;

    bufferId getId() const
    {
        bufferId rv;
        logOptiXContextError(rtBufferGetId(m_native_optix_buffer.get(), &rv.native));
        return rv;
    }

private:
    #include "optix_buffer_tuner.inl"
    using my_tuner_type = OptiXBufferTuner<T>;


    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width) :
        HasContractWithOptiXContext{ optix_context }
    {
        m_native_optix_buffer.reset(my_tuner_type::create_buffer(*this, buffer_kind),
            [](RTbuffer b) -> void
        {
            logOptiXContextError(rtBufferDestroy(b));
        });
        logOptiXContextError(rtBufferSetSize1D(m_native_optix_buffer.get(), width));
    }

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width, size_t height) :
        HasContractWithOptiXContext{ optix_context }
    {
        m_native_optix_buffer.reset(my_tuner_type::create_buffer(*this, buffer_kind),
            [](RTbuffer b) -> void
        {
            logOptiXContextError(rtBufferDestroy(b));
        });
        logOptiXContextError(rtBufferSetSize2D(m_native_optix_buffer.get(), width, height));
    }

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width, size_t height, size_t depth) :
        HasContractWithOptiXContext{ optix_context }
    {
        m_native_optix_buffer.reset(my_tuner_type::create_buffer(*this, buffer_kind),
            [](RTbuffer b) -> void
        {
            logOptiXContextError(rtBufferDestroy(b));
        });
        logOptiXContextError(rtBufferSetSize3D(m_native_optix_buffer.get(), width, height, depth));
    }

public:
    void* map(OptiXBufferMapKind map_kind, unsigned int mipmap_level) const
    {
        void* rv{ nullptr };
        logOptiXContextError(rtBufferMapEx(m_native_optix_buffer.get(), static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
        return rv;
    }

    void unmap(unsigned int mipmap_level)
    {
        logOptiXContextError(rtBufferUnmapEx(m_native_optix_buffer.get(), mipmap_level));
    }

private:
    std::shared_ptr<RTbuffer_api> m_native_optix_buffer;
};


template<> class OptiXBufferAttorney<OptiXContext>
{
    friend class OptiXContext;

    template<typename T>
    static OptiXBuffer<T> createOptiXBuffer(OptiXContext const& optix_context, OptiXBufferKind buffer_kind, size_t width)
    {
        return OptiXBuffer<T>{ optix_context, buffer_kind, width };
    }

    template<typename T>
    static OptiXBuffer<T> createOptiXBuffer(OptiXContext const& optix_context, OptiXBufferKind buffer_kind, size_t width, size_t height)
    {
        return OptiXBuffer<T>{ optix_context, buffer_kind, width, height };
    }

    template<typename T>
    static OptiXBuffer<T> createOptiXBuffer(OptiXContext const& optix_context, OptiXBufferKind buffer_kind, size_t width, size_t height, size_t depth)
    {
        return OptiXBuffer<T>{ optix_context, buffer_kind, width, height, depth };
    }
};


}

#endif
