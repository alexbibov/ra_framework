#include "miss_shader.h"

using namespace ox_wrapper;

OxMissShader::OxMissShader(OxProgram const& miss_shader, OxRayType ray_type):
    OxContractWithOxContext{ miss_shader.context() },
    OxContractWithOxPrograms{ miss_shader },
    m_ray_type{ ray_type }
{
}

OxProgram OxMissShader::getProgram() const
{
    return getOxProgramFromDeclarationOffset();
}

OxRayType OxMissShader::rayType() const
{
    return m_ray_type;
}

bool OxMissShader::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtProgramValidate(nativeOptiXProgramHandle()));
    return res == RT_SUCCESS;
}

void OxMissShader::apply(OxObjectHandle top_scene_object) const
{
    THROW_OPTIX_ERROR(
        nativeOptiXContextHandle(),
        rtContextSetMissProgram(nativeOptiXContextHandle(), static_cast<unsigned int>(m_ray_type), nativeOptiXProgramHandle())
    );

    getOxProgramFromDeclarationOffset().setVariableValue("ox_entry_node", top_scene_object);
}
