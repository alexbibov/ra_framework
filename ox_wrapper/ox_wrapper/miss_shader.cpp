#include "miss_shader.h"

using namespace ox_wrapper;

OxMissShader::OxMissShader(OxProgram const& miss_shader, OxRayType ray_type):
    OxContractWithOxContext{ miss_shader.context() },
    OxContractWithOxPrograms{ miss_shader },
    m_ray_type{ ray_type }
{
}

OxProgram OxMissShader::getMissShader() const
{
    return getOxProgramFromDeclarationOffset();
}

OxRayType OxMissShader::rayType() const
{
    return m_ray_type;
}

bool OxMissShader::isValid() const
{
    RTresult res = rtProgramValidate(nativeOptiXProgramHandle());
    logOptiXContextError(res);
    return res == RT_SUCCESS;
}

void OxMissShader::apply() const
{
    throwOptiXContextError(rtContextSetMissProgram(nativeOptiXContextHandle(), static_cast<unsigned int>(m_ray_type), nativeOptiXProgramHandle()));
}