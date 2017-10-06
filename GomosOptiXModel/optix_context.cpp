#include "optix_context.h"
#include "log.h"

using namespace ox_wrapper;

OptiXContext::OptiXContext(uint32_t num_entry_points/* = 1U*/)
{
    RTresult optix_rc = rtContextCreate(&m_optix_context);

    if (optix_rc == RT_ERROR_NO_DEVICE)
    {
        Log::retrieve()->out("OptiX is not supported by the GPU installed in the host system", LogMessageType::error);
        throw std::runtime_error("OptiX is not supported");
    }

    if (optix_rc == RT_ERROR_INVALID_VALUE)
    {
        Log::retrieve()->out("OptiX context cannot be initialized: invalid value", LogMessageType::error);
        throw std::runtime_error("invalid value");
    }

    if (!(m_is_context_valid = validate()))
        Log::retrieve()->out("The OptiX context is invalid: " + getValidationDescription(), LogMessageType::error);

    LOG_OPTIX_ERROR(m_optix_context, rtContextSetEntryPointCount(m_optix_context, num_entry_points));
}

OptiXContext::~OptiXContext()
{
    LOG_OPTIX_ERROR(m_optix_context, rtContextDestroy(m_optix_context));
}

RTcontext OptiXContext::native() const
{
    return m_optix_context;
}

bool OptiXContext::validate() const
{
    m_validation_result = rtContextValidate(m_optix_context);
    return m_validation_result == RT_SUCCESS;
}

std::string OptiXContext::getValidationDescription() const
{
    char const* error_string;
    rtContextGetErrorString(m_optix_context, m_validation_result, &error_string);

    return error_string;
}

OptiXProgram OptiXContext::createProgram(std::string const& source, OptiXProgram::Source source_type, std::string const& program_name) const
{
    return OptiXProgram{ *this, source, source_type, program_name };
}

RTcontext OptiXContext::get_native_context() const
{
    return m_optix_context;
}
