#include "ray_generator.h"
#include "context.h"

using namespace ox_wrapper;


OxRayGenerator::OxRayGenerator(OxProgram const& ray_generation_shader, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, 
    uint32_t entry_point_index):
    OxContractWithOxContext{ ray_generation_shader.context() },
    OxContractWithOxPrograms{ ray_generation_shader },
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
    m_miss_shader_assembly{ miss_shader_assembly },
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
    return m_miss_shader_assembly;
}

void OxRayGenerator::setMissShaderAssembly(OxMissShaderAssembly const& miss_shader_assembly)
{
    m_miss_shader_assembly = miss_shader_assembly;
}

bool OxRayGenerator::isValid() const
{
    RTresult res = rtProgramValidate(nativeOptiXProgramHandle());
    logOptiXContextError(res);
    return res == RT_SUCCESS && m_miss_shader_assembly.isValid();
}

void OxRayGenerator::update() const
{
    throwOptiXContextError(rtContextSetRayGenerationProgram(nativeOptiXContextHandle(), m_entry_point_index, nativeOptiXProgramHandle()));
    OxMissShaderAssemblyAttorney<OxRayGenerator>::applyMissShaderAssembly(m_miss_shader_assembly);
}

void OxRayGenerator::launch() const
{
    if (m_num_rays_z > 1U)
    {
        throwOptiXContextError(rtContextLaunch3D(
            nativeOptiXContextHandle(),
            m_entry_point_index,
            m_num_rays_x, m_num_rays_y, m_num_rays_z));
    }
    else if (m_num_rays_y > 1U)
    {
        throwOptiXContextError(rtContextLaunch2D(
            nativeOptiXContextHandle(),
            m_entry_point_index,
            m_num_rays_x, m_num_rays_y));
    }
    else
    {
        throwOptiXContextError(rtContextLaunch1D(
            nativeOptiXContextHandle(),
            m_entry_point_index, 
            m_num_rays_x));
    }
}
