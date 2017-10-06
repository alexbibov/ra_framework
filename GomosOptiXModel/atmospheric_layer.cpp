#include "atmospheric_layer.h"
#include "ptx_modules.h"
#include "optix_context.h"

using namespace gomos_optix_model;
using namespace ox_wrapper;

AtmosphericLayer::AtmosphericLayer(OptiXContext& optix_context, float planet_radius, float height, float density,
    OptiXBuffer<RayPayload> const& ray_payload_buffer, OptiXBuffer<ConvergedRayPayload> const& converged_rays_buffer):
    m_planet_radius{ planet_radius },
    m_height{ height },
    m_density{ density },
    m_backend_program{ optix_context.createProgram(PTX_ATMOSPHERIC_LAYER, OptiXProgram::Source::file, 
        "AtmosphericLayer_H" + std::to_string(height)) },
    m_ray_marching_buffer{ ray_payload_buffer },
    m_converged_rays_buffer{ converged_rays_buffer }
{
    m_backend_program.declareVariable("height", m_height);
    m_backend_program.declareVariable("planet_radius", m_planet_radius);
    m_backend_program.declareVariable("recursion_depth", m_max_ray_marching_recursion_depth);
    m_backend_program.assignBuffer("ray_marching_buffer", m_ray_marching_buffer);
    m_backend_program.assignBuffer("converged_rays_buffer", m_converged_rays_buffer);
}

float AtmosphericLayer::height() const
{
    return m_height;
}

float AtmosphericLayer::density() const
{
    return m_density;
}

void AtmosphericLayer::setMaximumRayMarchingRecursionDepth(uint16_t max_recursion_depth)
{
    m_max_ray_marching_recursion_depth = max_recursion_depth;
    m_backend_program.setVariableValue("recursion_depth", m_max_ray_marching_recursion_depth);
}

uint16_t AtmosphericLayer::getMaximumRayMarchingRecursionDepth() const
{
    return m_max_ray_marching_recursion_depth;
}
