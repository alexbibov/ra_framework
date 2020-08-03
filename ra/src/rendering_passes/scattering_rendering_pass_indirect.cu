#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "constants.h"
#include "ray_payloads.h"


rtDeclareVariable(rtObject, ra_entry_node, , "Scene entry node");


rtDeclareVariable(float, step_size, , "Ray marching step size");
rtDeclareVariable(optix::uint, max_recursion_depth, , "Maximal depth of recursion for scattering traverse");
rtDeclareVariable(float, intersection_distance, rtIntersectionDistance, "Parametric distance from ray origin to the intersection");
rtDeclareVariable(optix::Ray, current_ray, rtCurrentRay, "Currently traversed ray");
rtDeclareVariable(ra::RaRayRadiancePayloadSimple, ray_payload_scattered, rtPayload, "Payload of the current scattering ray");

typedef rtCallableProgramId<optix::float2(optix::float3, unsigned int)> factor_program_id_type;
rtDeclareVariable(factor_program_id_type, absorption_factor, , );


using namespace optix;
using namespace ra;


__device__ void update_ray_payload_scattered(float3 p_2)
{
    unsigned int spectrum = ray_payload_scattered.tracing_depth_and_aux.w;
    float2 phi = expf(-absorption_factor(p_2, spectrum) * step_size);
    ray_payload_scattered.spectral_radiance *= phi;
}

RT_PROGRAM void __ra_any_hit__(void)
{
    printf("Closest hit scattered\n");
    // this shader is only called when scattered ray exits the domain of the medium
    /*unsigned int idb_offset = ray_payload_scattered.tracing_depth_and_aux.z;

    float2 incoming_spectral_radiance = importance_directions_buffer[idb_offset];
    ray_payload_scattered.spectral_radiance = incoming_spectral_radiance;*/
}

RT_PROGRAM void __ra_miss__(void)
{
    // this shader is only invoked from within the medium
    // printf("Miss scattered\n");

    ++ray_payload_scattered.tracing_depth_and_aux.x;

    if (ray_payload_scattered.tracing_depth_and_aux.x < max_recursion_depth)
    {
        float3 p{ current_ray.origin + step_size * current_ray.direction };
        float3 p_2{ current_ray.origin + step_size * .5f * current_ray.direction };

        float d = RT_DEFAULT_MAX;

        Ray next_scattered_ray_iteration = make_Ray(
            p, current_ray.direction,
            static_cast<unsigned int>(RaRayType::scattered), 0.f, d);
        rtTrace(ra_entry_node, next_scattered_ray_iteration, ray_payload_scattered);

        update_ray_payload_scattered(p_2);
    }
}