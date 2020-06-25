#include "miss_shader.h"

using namespace ra;

RaMissShader::RaMissShader(RaProgram const& miss_shader, 
    RaRayTypeCollection const& supported_ray_types/* = RaRayTypeCollection{ 1U, RaRayType::unknown }*/):
    RaContractWithRaContext{ miss_shader.context() },
    RaContractWithRaPrograms{ miss_shader },
    m_supported_ray_types{ supported_ray_types }
{
}

RaProgram RaMissShader::getProgram() const
{
    return getRaProgramFromDeclarationOffset();
}

RaRayTypeCollection RaMissShader::supportedRayTypes() const
{
    return m_supported_ray_types;
}

bool RaMissShader::supportsRayType(RaRayType ray_type) const
{
    return std::find(m_supported_ray_types.begin(), m_supported_ray_types.end(), ray_type)
        != m_supported_ray_types.end();
}

bool RaMissShader::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtProgramValidate(nativeOptiXProgramHandle()));
    return res == RT_SUCCESS;
}

void RaMissShader::apply(RaObjectHandle top_scene_object) const
{
    for(auto rt : m_supported_ray_types)
    {
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtContextSetMissProgram(nativeOptiXContextHandle(), static_cast<unsigned int>(rt), nativeOptiXProgramHandle())
        );
    }

    getRaProgramFromDeclarationOffset().setVariableValue("ra_entry_node", top_scene_object);
}
