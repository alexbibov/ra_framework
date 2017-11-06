#ifndef OX_WRAPPER_HAS_CONTRACT_WITH_OPTIX_CONTEXT_H

#include "optix.h"
#include "ox_wrapper_fwd.h"

namespace ox_wrapper {

    class HasContractWithOptiXContext
    {
    public:
        OptiXContext const& context() const;

    protected:
        HasContractWithOptiXContext(OptiXContext const& optix_context_wrapper);

        RTcontext nativeOptiXContextHandle() const;
        void logOptiXContextError(RTresult error_code) const;

    private:
        OptiXContext const& m_optix_context_wrapper_ref;
    };

}

#define OX_WRAPPER_HAS_CONTRACT_WITH_OPTIX_CONTEXT_H
#endif
