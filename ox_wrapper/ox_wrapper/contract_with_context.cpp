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

OxContractWithOxContext& OxContractWithOxContext::operator=(OxContractWithOxContext const& other)
{
    if (&m_optix_context_wrapper_ref != &other.m_optix_context_wrapper_ref)
        THROW_OX_WRAPPER_ERROR("Unable to assign ox_wrapper objects belonging to different contexts");

    return *this;
}

RTcontext OxContractWithOxContext::nativeOptiXContextHandle() const
{
    return m_optix_context_wrapper_ref.m_optix_context;
}
