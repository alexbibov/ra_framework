#ifndef OX_WRAPPER_CONTRACT_WITH_CONTEXT_H
#define OX_WRAPPER_CONTRACT_WITH_CONTEXT_H

#include "optix.h"
#include "ox_wrapper_fwd.h"

namespace ox_wrapper {

    class OxContractWithOxContext
    {
    public:
        OxContext const& context() const;

    protected:
        OxContractWithOxContext(OxContext const& optix_context_wrapper);
        virtual ~OxContractWithOxContext() = default;

        OxContractWithOxContext& operator=(OxContractWithOxContext const& other);

        RTcontext nativeOptiXContextHandle() const;

    private:
        OxContext const& m_optix_context_wrapper_ref;
    };

}

#endif
