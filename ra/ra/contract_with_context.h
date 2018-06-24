#ifndef OX_WRAPPER_CONTRACT_WITH_CONTEXT_H
#define OX_WRAPPER_CONTRACT_WITH_CONTEXT_H

#include "optix.h"
#include "ra_fwd.h"

namespace ra {

    class RaContractWithRaContext
    {
    public:
        RaContext const& context() const;

    protected:
        RaContractWithRaContext(RaContext const& optix_context_wrapper);
        virtual ~RaContractWithRaContext() = default;

        RaContractWithRaContext& operator=(RaContractWithRaContext const& other);

        RTcontext nativeOptiXContextHandle() const;

    private:
        RaContext const& m_optix_context_wrapper_ref;
    };

}

#endif
