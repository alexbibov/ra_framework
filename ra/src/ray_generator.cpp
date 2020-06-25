#include "ray_generator.h"
#include "context.h"

using namespace ra;


RaRayGenerator::RaRayGenerator(RaProgram const& ray_generation_shader, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, 
    uint32_t entry_point_index):
    RaContractWithRaContext{ ray_generation_shader.context() },
    RaContractWithRaPrograms{ ray_generation_shader },
    m_p_miss_shader_assembly{ new util::Optional<RaMissShaderAssembly>{} },
    m_num_rays_x{ num_rays_x },
    m_num_rays_y{ num_rays_y },
    m_num_rays_z{ num_rays_z },
    m_entry_point_index{ entry_point_index }
{
}

RaRayGenerator::RaRayGenerator(RaProgram const& ray_generation_shader, RaMissShaderAssembly const& miss_shader_assembly,
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, uint32_t entry_point_index):
    RaContractWithRaContext{ ray_generation_shader.context() },
    RaContractWithRaPrograms{ ray_generation_shader },
    m_p_miss_shader_assembly{ new util::Optional<RaMissShaderAssembly>{miss_shader_assembly} },
    m_num_rays_x{ num_rays_x },
    m_num_rays_y{ num_rays_y },
    m_num_rays_z{ num_rays_z },
    m_entry_point_index{ entry_point_index }
{
}

void RaRayGenerator::setGeneratorDimensions(uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z)
{
    m_num_rays_x = num_rays_x;
    m_num_rays_y = num_rays_y;
    m_num_rays_z = num_rays_z;
}

RaProgram RaRayGenerator::getRayGenerationShader() const
{
    return getRaProgramFromDeclarationOffset();
}

util::Optional<RaMissShaderAssembly> RaRayGenerator::getMissShaderAssembly() const
{
    return *m_p_miss_shader_assembly;
}

void RaRayGenerator::setMissShaderAssembly(RaMissShaderAssembly const& miss_shader_assembly) const
{
    *m_p_miss_shader_assembly = miss_shader_assembly;
}

bool RaRayGenerator::isValid() const
{
    bool has_miss_shader_assembly_attached;
    if (!(has_miss_shader_assembly_attached = m_p_miss_shader_assembly->isValid()))
    {
        util::Log::retrieve()->out("Ray generator \"" + getStringName() + "\" does not have miss shader assembly attached", util::LogMessageType::exclamation);
        return false;
    }

    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtProgramValidate(nativeOptiXProgramHandle()));
    return has_miss_shader_assembly_attached ?
        res == RT_SUCCESS && static_cast<RaMissShaderAssembly&>(*m_p_miss_shader_assembly).isValid() :
        res == RT_SUCCESS;
}

uint3 RaRayGenerator::getGeneratorDimensions() const
{
    return uint3{ m_num_rays_x, m_num_rays_y, m_num_rays_z };
}

unsigned int RaRayGenerator::numberOfRays() const
{
    return m_num_rays_x * m_num_rays_y*m_num_rays_z;
}

void RaRayGenerator::update(RaObjectHandle top_scene_object) const
{
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
        rtContextSetRayGenerationProgram(nativeOptiXContextHandle(), m_entry_point_index, nativeOptiXProgramHandle()));

    if (m_p_miss_shader_assembly->isValid()
        && static_cast<RaMissShaderAssembly&>(*m_p_miss_shader_assembly).isValid())
        RaMissShaderAssemblyAttorney<RaRayGenerator>::applyMissShaderAssembly(*m_p_miss_shader_assembly, top_scene_object);
}

void RaRayGenerator::launch() const
{
    if (m_num_rays_z > 1U)
    {
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtContextLaunch3D(
            nativeOptiXContextHandle(),
            m_entry_point_index,
            m_num_rays_x, m_num_rays_y, m_num_rays_z));
    }
    else if (m_num_rays_y > 1U)
    {
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtContextLaunch2D(
            nativeOptiXContextHandle(),
            m_entry_point_index,
            m_num_rays_x, m_num_rays_y));
    }
    else
    {
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtContextLaunch1D(
            nativeOptiXContextHandle(),
            m_entry_point_index, 
            m_num_rays_x));
    }
}

void RaRayGenerator::update_topology(uint32_t new_num_rays_x, 
    uint32_t new_num_rays_y, 
    uint32_t new_num_rays_z, 
    uint32_t new_entry_point_index) const
{
    m_num_rays_x = new_num_rays_x;
    m_num_rays_y = new_num_rays_y;
    m_num_rays_z = new_num_rays_z;

    m_entry_point_index = new_entry_point_index;
}
