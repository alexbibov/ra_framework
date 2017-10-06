#include "optix_ray_generator.h"
#include "optix_context.h"

using namespace ox_wrapper;

OptiXRayGenerator::OptiXRayGenerator(OptiXProgram const& ray_generation_program,
    uint32_t numRays_x/* = 1U*/, uint32_t numRays_y/* = 1U*/, uint32_t num_Rays_z/* = 1U*/,
    uint32_t entry_point_index/* = 0U*/):
    m_optix_context{ ray_generation_program.getOptiXContext() },
    m_ray_generation_program{ ray_generation_program },
    m_num_rays_x{ numRays_x },
    m_num_rays_y{ numRays_y },
    m_num_rays_z{ num_Rays_z },
    m_entry_point_index{ entry_point_index }
{

}

void OptiXRayGenerator::launch() const
{
    if (m_num_rays_z > 1U)
    {
        LOG_OPTIX_ERROR(m_optix_context.native(),
            rtContextLaunch3D(m_optix_context.native(),
                m_entry_point_index,
                m_num_rays_x, m_num_rays_y, m_num_rays_z));
    }
    else if (m_num_rays_y > 1U)
    {
        LOG_OPTIX_ERROR(m_optix_context.native(),
            rtContextLaunch2D(m_optix_context.native(),
                m_entry_point_index,
                m_num_rays_x, m_num_rays_y));
    }
    else
    {
        LOG_OPTIX_ERROR(m_optix_context.native(),
            rtContextLaunch1D(m_optix_context.native(), 
                m_entry_point_index, 
                m_num_rays_x));
    }
}
