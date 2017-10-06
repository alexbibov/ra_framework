#ifndef GOMOS_OPTIX_MODEL_RAY_PAYLOAD_H
#define GOMOS_OPTIX_MODEL_RAY_PAYLOAD_H

#include "../CUDA/v9.0/include/vector_types.h"

namespace gomos_optix_model {

struct RayPayload
{
    unsigned int index;    // index of the ray being marched
    float intensity;    // intensity carried by the ray
    float3 ray_marching_last_step;    // ray origin to be used on the next iteration 
    unsigned int generation;    // generation of the ray
};

struct ConvergedRayPayload
{
    unsigned int index;    // index of the ray in original indexing system
    float intensity;    // intensity carried by the ray
};

}

#endif
