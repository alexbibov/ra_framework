#ifndef OX_WRAPPER_OPTIX_BUFFER_H
#define OX_WRAPPER_OPTIX_BUFFER_H

#include "optix.h"
#include "log.h"
#include "../CUDA/v9.0/include/vector_types.h"

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
class OptiXBuffer
{
    friend class OptiXContext;


public:

    ~OptiXBuffer()
    {
        LOG_OPTIX_ERROR(m_optix_context.native(), rtBufferDestroy(m_native_optix_buffer));
    }

    RTbuffer native() const
    {
        return m_native_optix_buffer;
    }

    bufferId getId() const
    {
        bufferId rv;
        LOG_OPTIX_ERROR(m_optix_context.native(), rtBufferGetId(m_native_optix_buffer, &rv.native));
        return rv;
    }

private:

#include "optix_buffer_tuner.inl"


private:

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width) :
        m_optix_context{ optix_context }
    {
        OptiXBufferTuner<T>::create_buffer(optix_context.native(), buffer_kind);
        LOG_OPTIX_ERROR(optix_context.native(), 
            rtBufferSetSize1D(m_native_optix_buffer, width));
    }

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width, size_t height) :
        m_optix_context{ optix_context }
    {
        OptiXBufferTuner<T>::create_buffer(optix_context.native(), buffer_kind);
        LOG_OPTIX_ERROR(optix_context.native(), 
            rtBufferSetSize2D(m_native_optix_buffer, width, height));
    }

    OptiXBuffer(OptiXContext const& optix_context,
        OptiXBufferKind buffer_kind, size_t width, size_t height, size_t depth) :
        m_optix_context{ optix_context }
    {
        OptiXBufferTuner<T>::create_buffer(optix_context.native(), buffer_kind);
        LOG_OPTIX_ERROR(optix_context.native(), 
            rtBufferSetSize3D(m_native_optix_buffer, width, height, depth));
    }

    void* map(OptiXBufferMapKind map_kind, unsigned int mipmap_level) const
    {
        void* rv{ nullptr };
        LOG_OPTIX_ERROR(m_optix_context.native(), 
            rtBufferMapEx(m_native_optix_buffer, static_cast<unsigned>(map_kind), mipmap_level, nullptr, &rv));
        return rv;
    }

    void unmap(unsigned int mipmap_level)
    {
        LOG_OPTIX_ERROR(m_optix_context.native(), 
            rtBufferUnmapEx(m_native_optix_buffer, mipmap_level));
    }

private:

    OptiXContext const& m_optix_context;
    RTbuffer m_native_optix_buffer;
};

}

#endif
