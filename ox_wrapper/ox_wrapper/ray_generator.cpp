#include "ray_generator.h"
#include "context.h"

using namespace ox_wrapper;

OxRayGenerator::OxRayGenerator(OxProgram const& optix_ray_generation_shader,
    uint32_t num_rays_x/* = 1U*/, uint32_t num_rays_y/* = 1U*/, uint32_t num_rays_z/* = 1U*/,
    uint32_t entry_point_index/* = 0U*/):
    OxContractWithOxContext{ optix_ray_generation_shader.context() },
    OxContractWithOxPrograms{ optix_ray_generation_shader },
    m_num_rays_x{ num_rays_x },
    m_num_rays_y{ num_rays_y },
    m_num_rays_z{ num_rays_z },
    m_entry_point_index{ entry_point_index }
{
    
}

OxRayGenerator::OxRayGenerator(OxProgram const& optix_ray_generation_shader, OxProgram const& optix_miss_shader, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, uint32_t entry_point_index):
    OxContractWithOxContext{ optix_ray_generation_shader.context() },
    OxContractWithOxPrograms{ optix_ray_generation_shader, optix_miss_shader },
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
    return getOxProgramFromDeclarationOffset(0U);
}

OxProgram OxRayGenerator::getMissShader() const
{
    return getOxProgramFromDeclarationOffset(1U);
}

bool OxRayGenerator::isValid() const
{
    RTresult res = rtProgramValidate(nativeOptiXProgramHandle(0U));
    logOptiXContextError(res);
    return res == RT_SUCCESS;
}

void OxRayGenerator::update() const
{
    throwOptiXContextError(rtContextSetRayGenerationProgram(nativeOptiXContextHandle(), m_entry_point_index, nativeOptiXProgramHandle(0U)));
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
