#include "optix_context.h"
#include "ox_exception.h"

using namespace ox_wrapper;

OptiXContext::OptiXContext(uint32_t num_entry_points/* = 1U*/):
    m_error_state{ false }
{
    RTcontext context_handle;
    RTresult optix_rc = rtContextCreate(&context_handle);

    if (optix_rc == RT_ERROR_NO_DEVICE)
    {
        throw OxException{ "OptiX is not supported by the GPU installed in the host system" };
    }

    if (optix_rc == RT_ERROR_INVALID_VALUE)
    {
        throw OxException{ "OptiX context cannot be initialized: invalid value" };
    }

    logError(rtContextValidate(context_handle));
    logError(rtContextSetEntryPointCount(context_handle, num_entry_points));

    m_optix_context.reset(context_handle,
        [](RTcontext h) -> void
    {
        LOG_OPTIX_ERROR(h, rtContextDestroy(h));
    });
}

OptiXProgram OptiXContext::createProgram(std::string const& source, OptiXProgram::Source source_type, std::string const& program_name) const
{
    return OptiXProgram{ *this, source, source_type, program_name };
}

void OptiXContext::logError(RTresult error_code) const
{
    if ((m_error_state = error_code) != RT_SUCCESS)
        LOG_OPTIX_ERROR(*m_optix_context, error_code);
}

OptiXContext::operator bool() const
{
    return !hasErrors();
}

bool OptiXContext::hasErrors() const
{
    return m_error_state != RT_SUCCESS;
}

OptiXContext const& HasContractWithOptiXContext::context() const
{
    return m_optix_context_wrapper;
}

HasContractWithOptiXContext::HasContractWithOptiXContext(OptiXContext const& optix_context_wrapper):
    m_optix_context_wrapper{ optix_context_wrapper }
{

}

RTcontext HasContractWithOptiXContext::nativeOptiXContextHandle() const
{
    return *m_optix_context_wrapper.m_optix_context;
}

void HasContractWithOptiXContext::logOptiXContextError(RTresult error_code) const
{
    m_optix_context_wrapper.logError(error_code);
}
