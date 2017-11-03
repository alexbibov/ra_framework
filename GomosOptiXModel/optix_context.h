#ifndef OX_WRAPPER_OPTIX_CONTEXT_H
#define OX_WRAPPER_OPTIX_CONTEXT_H

#include <string>
#include <memory>

#include "optix.h"
#include "optix_program.h"
#include "optix_buffer.h"

namespace ox_wrapper {

//! Simple wrapper over the OptiX context
class OptiXContext final
{
    friend class HasContractWithOptiXContext;

public:

    OptiXContext(uint32_t num_entry_points = 1U);
    ~OptiXContext() = default;

    //! Creates new OptiX program
    OptiXProgram createProgram(std::string const& source, OptiXProgram::Source source_type, std::string const& program_name) const;

    //! Creates new 1D OptiX buffer
    template<typename T>
    OptiXBuffer<T> createBuffer(OptiXBufferKind buffer_kind, size_t width) const
    {
        return OptiXBuffer<T>{ *this, buffer_kind, width };
    }

    //! Creates new 2D OptiX buffer
    template<typename T>
    OptiXBuffer<T> createBuffer(OptiXBufferKind buffer_kind, size_t width, size_t height) const
    {
        return OptiXBuffer<T>{ *this, buffer_kind, width, height };
    }

    //! Creates new 3D OptiX buffer
    template<typename T>
    OptiXBuffer<T> createBuffer(OptiXBufferKind buffer_kind, size_t width, size_t height, size_t depth) const
    {
        return OptiXBuffer<T>{ *this, buffer_kind, width, height, depth };
    }

    //! Returns 'true' in case if OptiX context is NOT in error state; returns 'false' otherwise
    operator bool() const;

    //! Returns 'true' in case if OptiX context IS in erroneous state; returns 'false' otherwise
    bool hasErrors() const;

private:
    //! Logs out OptiX error message based on error code. If supplied value is RT_SUCCESS, the function has no effect
    void logError(RTresult error_code) const;

private:

    std::shared_ptr<RTcontext> m_optix_context;
    mutable RTresult m_error_state;
};


class HasContractWithOptiXContext
{
public:
    OptiXContext const& context() const;

protected:
    HasContractWithOptiXContext(OptiXContext const& optix_context_wrapper);

    RTcontext nativeOptiXContextHandle() const;
    void logOptiXContextError(RTresult error_code) const;

private:
    OptiXContext m_optix_context_wrapper;
};


}

#endif
