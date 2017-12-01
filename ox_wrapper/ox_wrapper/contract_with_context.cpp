#include "contract_with_context.h"
#include "context.h"

using namespace ox_wrapper;

OxContext const& OxContractWithOxContext::context() const
{
    return m_optix_context_wrapper_ref;
}

OxContractWithOxContext::OxContractWithOxContext(OxContext const& optix_context_wrapper) :
    m_optix_context_wrapper_ref{ optix_context_wrapper }
{

}

RTcontext OxContractWithOxContext::nativeOptiXContextHandle() const
{
    return m_optix_context_wrapper_ref.m_optix_context;
}

void OxContractWithOxContext::logOptiXContextError(RTresult error_code) const
{
    m_optix_context_wrapper_ref.logError(error_code);
}

void OxContractWithOxContext::throwOptiXContextError(RTresult error_code) const
{
    m_optix_context_wrapper_ref.throwError(error_code);
}
