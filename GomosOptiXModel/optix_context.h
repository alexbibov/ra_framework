#ifndef OX_WRAPPER_OPTIX_CONTEXT_H
#define OX_WRAPPER_OPTIX_CONTEXT_H

#include <string>

#include "optix.h"
#include "optix_program.h"
#include "optix_buffer.h"

namespace ox_wrapper {

//! Simple wrapper over the OptiX context
class OptiXContext
{
public:

    OptiXContext(uint32_t num_entry_points = 1U);
    virtual ~OptiXContext();

    RTcontext native() const;

    /*! Validates the OptiX context and all of the OptiX objects associated with it
    for correctness and returns 'true' if the context is valid and 'false' otherwise.
    In order to obtain descriptive message containing the details regarding why the 
    OptiX state may be invalid use getValidationDescription()
    */
    bool validate() const;

    //! Returns human readable message describing validation results of the OptiX state
    std::string getValidationDescription() const;

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

private:

    RTcontext m_optix_context;
    mutable RTresult m_validation_result;
    bool m_is_context_valid;

    RTcontext get_native_context() const;
};

}

#endif
