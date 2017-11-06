#include "optix_ray_generator.h"
#include "optix_context.h"

using namespace ox_wrapper;

OptiXRayGenerator::OptiXRayGenerator(OptiXProgram const& optix_ray_generation_program,
    uint32_t numRays_x/* = 1U*/, uint32_t numRays_y/* = 1U*/, uint32_t num_Rays_z/* = 1U*/,
    uint32_t entry_point_index/* = 0U*/):
    HasContractWithOptiXContext{ optix_ray_generation_program.context() },
    HasContractWithOptiXProgram{ optix_ray_generation_program },
    m_num_rays_x{ numRays_x },
    m_num_rays_y{ numRays_y },
    m_num_rays_z{ num_Rays_z },
    m_entry_point_index{ entry_point_index }
{
    logOptiXContextError(rtContextSetRayGenerationProgram(nativeOptiXContextHandle(), m_entry_point_index, nativeOptiXProgramHandle()));
}

void OptiXRayGenerator::launch() const
{
    if (m_num_rays_z > 1U)
    {
        logOptiXContextError(rtContextLaunch3D(
            nativeOptiXContextHandle(),
            m_entry_point_index,
            m_num_rays_x, m_num_rays_y, m_num_rays_z));
    }
    else if (m_num_rays_y > 1U)
    {
        logOptiXContextError(rtContextLaunch2D(
            nativeOptiXContextHandle(),
            m_entry_point_index,
            m_num_rays_x, m_num_rays_y));
    }
    else
    {
        logOptiXContextError(rtContextLaunch1D(
            nativeOptiXContextHandle(), 
            m_entry_point_index, 
            m_num_rays_x));
    }
}
