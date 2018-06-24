#include "contract_with_context.h"
#include "context.h"

using namespace ra;

RaContext const& RaContractWithRaContext::context() const
{
    return m_optix_context_wrapper_ref;
}

RaContractWithRaContext::RaContractWithRaContext(RaContext const& optix_context_wrapper) :
    m_optix_context_wrapper_ref{ optix_context_wrapper }
{

}

RaContractWithRaContext& RaContractWithRaContext::operator=(RaContractWithRaContext const& other)
{
    if (&m_optix_context_wrapper_ref != &other.m_optix_context_wrapper_ref)
        THROW_OX_WRAPPER_ERROR("Unable to assign ra objects belonging to different contexts");

    return *this;
}

RTcontext RaContractWithRaContext::nativeOptiXContextHandle() const
{
    return m_optix_context_wrapper_ref.m_optix_context;
}
