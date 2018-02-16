#include "miss_shader.h"

using namespace ox_wrapper;

OxMissShader::OxMissShader(OxProgram const& miss_shader, 
    OxRayTypeCollection const& supported_ray_types/* = OxRayTypeCollection{ 1U, OxRayType::unknown }*/):
    OxContractWithOxContext{ miss_shader.context() },
    OxContractWithOxPrograms{ miss_shader },
    m_supported_ray_types{ supported_ray_types }
{
}

OxProgram OxMissShader::getProgram() const
{
    return getOxProgramFromDeclarationOffset();
}

OxRayTypeCollection OxMissShader::supportedRayTypes() const
{
    return m_supported_ray_types;
}

bool OxMissShader::supportsRayType(OxRayType ray_type) const
{
    return std::find(m_supported_ray_types.begin(), m_supported_ray_types.end(), ray_type)
        != m_supported_ray_types.end();
}

bool OxMissShader::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtProgramValidate(nativeOptiXProgramHandle()));
    return res == RT_SUCCESS;
}

void OxMissShader::apply(OxObjectHandle top_scene_object) const
{
    for(auto rt : m_supported_ray_types)
    {
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtContextSetMissProgram(nativeOptiXContextHandle(), static_cast<unsigned int>(rt), nativeOptiXProgramHandle())
        );
    }

    getOxProgramFromDeclarationOffset().setVariableValue("ox_entry_node", top_scene_object);
}
