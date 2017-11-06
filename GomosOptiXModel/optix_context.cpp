#include "optix_context.h"
#include "ox_exception.h"

using namespace ox_wrapper;

OptiXContext::OptiXContext(uint32_t num_entry_points/* = 1U*/):
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

    logError(rtContextValidate(m_optix_context));
    logError(rtContextSetEntryPointCount(m_optix_context, num_entry_points));
}

OptiXContext::~OptiXContext()
{
    logError(rtContextDestroy(m_optix_context));
}

OptiXProgram OptiXContext::createProgram(std::string const& source, OptiXProgram::Source source_type, std::string const& program_name) const
{
    return OptiXProgramAttorney<OptiXContext>::createOptiXProgram(*this, source, source_type, program_name);
}

void OptiXContext::logError(RTresult error_code) const
{
    if ((m_error_state = error_code) != RT_SUCCESS)
        LOG_OPTIX_ERROR(m_optix_context, error_code);
}

OptiXContext::operator bool() const
{
    return !hasErrors();
}

bool OptiXContext::hasErrors() const
{
    return m_error_state != RT_SUCCESS;
}