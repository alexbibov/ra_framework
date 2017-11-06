#ifndef OX_WRAPPER_HAS_CONTRACT_WITH_OPTIX_PROGRAM_H
#define OX_WRAPPER_HAS_CONTRACT_WITH_OPTIX_PROGRAM_H

#include "optix.h"
#include "ox_wrapper_fwd.h"

namespace ox_wrapper {
    class HasContractWithOptiXProgram 
    {
    public:
        OptiXProgram const& program() const;

    protected:
        HasContractWithOptiXProgram(OptiXProgram const& optix_program);

        RTprogram nativeOptiXProgramHandle() const;

    private:
        OptiXProgram const& m_optix_program;
    };
}

#endif
