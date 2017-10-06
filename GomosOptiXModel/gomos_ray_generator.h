#ifndef GOMOS_OPTIX_MODEL_GOMOS_RAY_GENERATOR_H
#define GOMOS_OPTIX_MODEL_GOMOS_RAY_GENERATOR_H

#include "optix_ray_generator.h"
#include "optix_program.h"
#include "ptx_modules.h"
#include "atmospheric_layer.h"

namespace gomos_optix_model {

class GomosRayGenerator : public ox_wrapper::OptiXRayGenerator {
public:
    GomosRayGenerator(ox_wrapper::OptiXContext const& context, 
        uint32_t num_rays, 
        AtmosphericLayer const& top_atmospheric_layer);

private:
    ox_wrapper::OptiXContext const& m_optix_context;
    AtmosphericLayer const& m_top_atmospheric_layer;
    uint32_t m_num_rays;
};

}

#endif
