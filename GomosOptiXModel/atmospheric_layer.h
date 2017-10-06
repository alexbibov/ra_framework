#ifndef GOMOS_OPTIX_MODEL_ATMOSPHERIC_LAYER_H
#define GOMOS_OPTIX_MODEL_ATMOSPHERIC_LAYER_H


#include "optix_program.h"
#include "optix_buffer.h"
#include "ray_payload.h"


namespace gomos_optix_model {

//! Defines single density layer of the atmosphere
class AtmosphericLayer
{
public:
    AtmosphericLayer(ox_wrapper::OptiXContext& optix_context, 
        float planet_radius, float height, float density,
        ox_wrapper::OptiXBuffer<RayPayload> const& ray_payload_buffer, 
        ox_wrapper::OptiXBuffer<ConvergedRayPayload> const& converged_rays_buffer);

    float height() const;
    float density() const;

    void setMaximumRayMarchingRecursionDepth(uint16_t max_recursion_depth);
    uint16_t getMaximumRayMarchingRecursionDepth() const;

private:
    float const m_planet_radius;
    float const m_height;
    float const m_density;
    uint16_t m_max_ray_marching_recursion_depth = 3U;

    ox_wrapper::OptiXProgram m_backend_program;
    ox_wrapper::OptiXBuffer<RayPayload> const& m_ray_marching_buffer;
    ox_wrapper::OptiXBuffer<ConvergedRayPayload> const& m_converged_rays_buffer;
};

}

#endif
