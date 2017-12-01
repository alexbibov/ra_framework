#include "context.h"
#include "exception.h"

using namespace ox_wrapper;

OxContext::OxContext(uint32_t num_entry_points/* = 1U*/):
    m_error_state{ RT_SUCCESS }
{
    RTresult optix_rc = rtContextCreate(&m_optix_context);

    if (optix_rc == RT_ERROR_NO_DEVICE)
    {
        throw OxException{ "OptiX is not supported by the GPU installed in the host system" };
    }

    if (optix_rc == RT_ERROR_INVALID_VALUE)
    {
        throw OxException{ "OptiX context cannot be initialized: invalid value" };
    }

    isValid();
    logError(rtContextSetEntryPointCount(m_optix_context, num_entry_points));
}

OxContext::~OxContext()
{
    logError(rtContextDestroy(m_optix_context));
}

OxProgram OxContext::createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const
{
    return OxProgramAttorney<OxContext>::createOptiXProgram(*this, source, source_type, program_name);
}

void OxContext::logError(RTresult error_code) const
{
    if ((m_error_state = error_code) != RT_SUCCESS)
        LOG_OPTIX_ERROR(m_optix_context, error_code);
}

void OxContext::throwError(RTresult error_code) const
{
    if ((m_error_state = error_code) != RT_SUCCESS)
        THROW_OPTIX_ERROR(m_optix_context, error_code);
}

OxContext::operator bool() const
{
    return !hasErrors();
}

bool OxContext::hasErrors() const
{
    return m_error_state != RT_SUCCESS;
}

bool ox_wrapper::OxContext::isValid() const
{
    RTresult res = rtContextValidate(m_optix_context);
    logError(res);
    return res == RT_SUCCESS;
}
