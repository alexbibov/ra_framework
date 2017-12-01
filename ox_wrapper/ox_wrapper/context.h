#ifndef OX_WRAPPER_CONTEXT_H
#define OX_WRAPPER_CONTEXT_H

#include <string>
#include <memory>

#include "optix.h"
#include "program.h"
#include "buffer.h"
#include "entity.h"

namespace ox_wrapper {

//! Simple wrapper over the OptiX context
class OxContext final : public OxEntity
{
    friend class OxContractWithOxContext;

public:

    OxContext(uint32_t num_entry_points = 1U);
    OxContext(OxContext const&) = delete;
    OxContext(OxContext&&) = delete;
    OxContext& operator=(OxContext const&) = delete;
    OxContext& operator=(OxContext&&) = delete;
    ~OxContext();

    //! Creates new OptiX program
    OxProgram createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const;

    //! Creates new 1D OptiX buffer
    template<typename T>
    OxBuffer<T> createBuffer(OxBufferKind buffer_kind, size_t width) const
    {
        return OptiXBufferAttorney<OxContext>::createOptiXBuffer(*this, buffer_kind, width);
    }

    //! Creates new 2D OptiX buffer
    template<typename T>
    OxBuffer<T> createBuffer(OxBufferKind buffer_kind, size_t width, size_t height) const
    {
        return OptiXBufferAttorney<OxContext>::createOptiXBuffer(*this, buffer_kind, width, height);
    }

    //! Creates new 3D OptiX buffer
    template<typename T>
    OxBuffer<T> createBuffer(OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) const
    {
        return OptiXBufferAttorney<OxContext>::createOptiXBuffer(*this, buffer_kind, width, height, depth);
    }

    //! Returns 'true' in case if OptiX context is NOT in error state; returns 'false' otherwise
    operator bool() const;

    //! Returns 'true' in case if OptiX context IS in erroneous state; returns 'false' otherwise
    bool hasErrors() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    //! Logs out OptiX error message based on error code. If supplied value is RT_SUCCESS, the function has no effect
    void logError(RTresult error_code) const;

    //! If error_code is not RT_SUCCESS, throws the corresponding exception and logs the human-readable message into the log file
    void throwError(RTresult error_code) const;

private:
    RTcontext m_optix_context;
    mutable RTresult m_error_state;
};

}

#endif
