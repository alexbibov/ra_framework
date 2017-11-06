#include "has_contract_with_optix_context.h"
#include "optix_context.h"

using namespace ox_wrapper;

OptiXContext const& HasContractWithOptiXContext::context() const
{
    return m_optix_context_wrapper_ref;
}

HasContractWithOptiXContext::HasContractWithOptiXContext(OptiXContext const& optix_context_wrapper) :
    m_optix_context_wrapper_ref{ optix_context_wrapper }
{

}

RTcontext HasContractWithOptiXContext::nativeOptiXContextHandle() const
{
    return m_optix_context_wrapper_ref.m_optix_context;
}

void HasContractWithOptiXContext::logOptiXContextError(RTresult error_code) const
{
    m_optix_context_wrapper_ref.logError(error_code);
}
