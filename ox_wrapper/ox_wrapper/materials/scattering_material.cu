#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/commons.h"
#include "ox_wrapper/ray_payload.h"



rtDeclareVariable(float, step_size, , "Ray marching step size");
typedef rtCallableProgramId<float(optix::float3)> absorption_factor_program_id_type;
typedef rtCallableProgramId<float(optix::float3)> scattering_factor_program_id_type;
typedef rtCallableProgramId<float(optix::float3, optix::float2, optix::float2)> phase_function_program_id_type; 
rtDeclareVariable(absorption_factor_program_id_type, absorption_factor, , );
rtDeclareVariable(scattering_factor_program_id_type, scattering_factor, , );
rtDeclareVariable(phase_function_program_id_type, phase_function, , );
rtDeclareVariable(unsigned int, num_importance_directions, , );

rtDeclareVariable(ox_wrapper::OxRayPayload, ray_payload, rtPayload, "Current ray payload");
rtDeclareVariable(float, intersection_distance, rtIntersectionDistance, "Parametric distance from ray origin to the intersection");
rtDeclareVariable(optix::Ray, current_ray, rtCurrentRay, "Currently traversed ray");

rtBuffer<float3, 1> importance_directions_buffer;


using namespace optix;
using namespace ox_wrapper;

RT_PROGRAM void __ox_intersect__(void)
{

}