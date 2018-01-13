#include "ray_generator.h"
#include "context.h"

using namespace ox_wrapper;


OxRayGenerator::OxRayGenerator(OxProgram const& ray_generation_shader, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, 
    uint32_t entry_point_index):
    OxContractWithOxContext{ ray_generation_shader.context() },
    OxContractWithOxPrograms{ ray_generation_shader },
    m_p_miss_shader_assembly{ new util::Optional<OxMissShaderAssembly>{} },
    m_num_rays_x{ num_rays_x },
    m_num_rays_y{ num_rays_y },
    m_num_rays_z{ num_rays_z },
    m_entry_point_index{ entry_point_index }
{
}

OxRayGenerator::OxRayGenerator(OxProgram const& ray_generation_shader, OxMissShaderAssembly const& miss_shader_assembly,
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, uint32_t entry_point_index):
    OxContractWithOxContext{ ray_generation_shader.context() },
    OxContractWithOxPrograms{ ray_generation_shader },
    m_p_miss_shader_assembly{ new util::Optional<OxMissShaderAssembly>{miss_shader_assembly} },
    m_num_rays_x{ num_rays_x },
    m_num_rays_y{ num_rays_y },
    m_num_rays_z{ num_rays_z },
    m_entry_point_index{ entry_point_index }
{
}

void OxRayGenerator::setGeneratorDimensions(uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z)
{
    m_num_rays_x = num_rays_x;
    m_num_rays_y = num_rays_y;
    m_num_rays_z = num_rays_z;
}

OxProgram OxRayGenerator::getRayGenerationShader() const
{
    return getOxProgramFromDeclarationOffset();
}

util::Optional<OxMissShaderAssembly> OxRayGenerator::getMissShaderAssembly() const
{
    return *m_p_miss_shader_assembly;
}

void OxRayGenerator::setMissShaderAssembly(OxMissShaderAssembly const& miss_shader_assembly) const
{
    *m_p_miss_shader_assembly = miss_shader_assembly;
}

bool OxRayGenerator::isValid() const
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
        res == RT_SUCCESS && static_cast<OxMissShaderAssembly&>(*m_p_miss_shader_assembly).isValid() :
        res == RT_SUCCESS;
}

uint3 OxRayGenerator::getGeneratorDimensions() const
{
    return uint3{ m_num_rays_x, m_num_rays_y, m_num_rays_z };
}

unsigned int OxRayGenerator::numberOfRays() const
{
    return m_num_rays_x * m_num_rays_y*m_num_rays_z;
}

void OxRayGenerator::update() const
{
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
        rtContextSetRayGenerationProgram(nativeOptiXContextHandle(), m_entry_point_index, nativeOptiXProgramHandle()));

    if (m_p_miss_shader_assembly->isValid()
        && static_cast<OxMissShaderAssembly&>(*m_p_miss_shader_assembly).isValid())
        OxMissShaderAssemblyAttorney<OxRayGenerator>::applyMissShaderAssembly(*m_p_miss_shader_assembly);
}

void OxRayGenerator::launch() const
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
