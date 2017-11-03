#ifndef OX_WRAPPER_OPTIX_BUFFER_H
#define OX_WRAPPER_OPTIX_BUFFER_H

#include "optix.h"
#include "log.h"
#include "../CUDA/v9.0/include/vector_types.h"

#include <memory>


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

template<typename T>
class OptiXBuffer : public HasContractWithOptiXContext
{
    friend class OptiXContext;

public:

    ~OptiXBuffer() = default;

    bufferId getId() const
    {
        bufferId rv;
        logOptiXContextError(rtBufferGetId(*m_native_optix_buffer, &rv.native));
        return rv;
    }

private:

    template<typename T> struct OptiXBufferTuner;
    #include "optix_buffer_tuner.inl"
    using my_tuner_type = OptiXBufferTuner<T>;


private:

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width) :
        HasContractWithOptiXContext{ optix_context }
    {
        my_tuner_type::create_buffer(*this, buffer_kind);
        logOptiXContextError(rtBufferSetSize1D(*m_native_optix_buffer, width));
    }

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width, size_t height) :
        HasContractWithOptiXContext{ optix_context }
    {
        my_tuner_type::create_buffer(*this, buffer_kind);
        logOptiXContextError(rtBufferSetSize2D(*m_native_optix_buffer, width, height));
    }

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width, size_t height, size_t depth) :
        HasContractWithOptiXContext{ optix_context }
    {
        my_tuner_type::create_buffer(*this, buffer_kind);
        logOptiXContextError(rtBufferSetSize3D(*m_native_optix_buffer, width, height, depth));
    }

    void* map(OptiXBufferMapKind map_kind, unsigned int mipmap_level) const
    {
        void* rv{ nullptr };
        logOptiXContextError(rtBufferMapEx(*m_native_optix_buffer, static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
        return rv;
    }

    void unmap(unsigned int mipmap_level)
    {
        logOptiXContextError(rtBufferUnmapEx(*m_native_optix_buffer, mipmap_level));
    }

private:
    std::shared_ptr<RTbuffer> m_native_optix_buffer;
};

}

#endif
