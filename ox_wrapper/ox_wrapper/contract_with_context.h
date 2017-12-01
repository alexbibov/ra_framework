#ifndef OX_WRAPPER_CONTRACT_WITH_CONTEXT_H
#define OX_WRAPPER_CONTRACT_WITH_CONTEXT_H

#include "optix.h"
#include "fwd.h"

namespace ox_wrapper {

    class OxContractWithOxContext
    {
    public:
        OxContext const& context() const;

    protected:
        OxContractWithOxContext(OxContext const& optix_context_wrapper);

        RTcontext nativeOptiXContextHandle() const;
        void logOptiXContextError(RTresult error_code) const;
        void throwOptiXContextError(RTresult error_code) const;

    private:
        OxContext const& m_optix_context_wrapper_ref;
    };

}

#endif
