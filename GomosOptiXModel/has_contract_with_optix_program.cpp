#include "has_contract_with_optix_program.h"
#include "optix_program.h"

using namespace ox_wrapper;

OptiXProgram const& HasContractWithOptiXProgram::program() const
{
    return m_optix_program;
}

HasContractWithOptiXProgram::HasContractWithOptiXProgram(OptiXProgram const& optix_program):
    m_optix_program{ optix_program }
{

}

RTprogram HasContractWithOptiXProgram::nativeOptiXProgramHandle() const
{
    return m_optix_program.m_native_optix_program.get();
}
