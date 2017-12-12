#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/commons.h"
#include "ox_wrapper/ray_radiance_payload.h"


rtDeclareVariable(rtObject, ox_entry_node, , "Scene entry node");

rtDeclareVariable(float, step_size, , "Ray marching step size");
rtDeclareVariable(unsigned int, max_recursion_depth, , "Maximal depth of recursion for scattering traverse");
rtDeclareVariable(unsigned int, max_scattering_depth, , "Maximal number of steps allowed for scattered rays");
rtDeclareVariable(unsigned int, num_spectra_supported, , "Number of wavelengths in use");
rtDeclareVariable(unsigned int, num_importance_directions, , );

typedef rtCallableProgramId<optix::float2(optix::float3, unsigned int)> absorption_factor_program_id_type;
typedef rtCallableProgramId<optix::float2(optix::float3, unsigned int)> scattering_factor_program_id_type;
typedef rtCallableProgramId<optix::float2(optix::float3, optix::float2, optix::float2, unsigned int)> phase_function_program_id_type; 
rtDeclareVariable(absorption_factor_program_id_type, absorption_factor, , );
rtDeclareVariable(scattering_factor_program_id_type, scattering_factor, , );
rtDeclareVariable(phase_function_program_id_type, phase_function, , );

rtDeclareVariable(ox_wrapper::OxRayRadiancePayload, ray_payload, rtPayload, "Current ray payload");
rtDeclareVariable(float, intersection_distance, rtIntersectionDistance, "Parametric distance from ray origin to the intersection");
rtDeclareVariable(optix::Ray, current_ray, rtCurrentRay, "Currently traversed ray");
rtDeclareVariable(optix::float3, index, rtLaunchIndex, "Index of the current ray");

rtDeclareVariable(optix::float3, normal, attribute attrNormal, "Normal of the surface being hit");


rtBuffer<optix::float2, 1> importance_directions_buffer;

/*! Traverse backup is a standard buffer, which enables implementation of multi-pass ray-tracing algorithms.
 The buffer has to be organized as follows:
 1st element contains number of vectors stored in the buffer, hereinafter denoted by N
 The rest of the buffer must contain at least 6*N floats and N unsigned integers ordered in 9-element
 tuples with the first 3 elements being floating point numbers determining the origin of the 
 corresponding ray, succeeding 3 elements being floating point components of the ray's direction, and
 the last 3 unsigned integers determining three-dimensional index of the ray in its original launch grid
*/
rtBuffer<unsigned int, 1> traverse_backup_buffer;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

using namespace optix;
using namespace ox_wrapper;

__device__ float sign(float x)
{
    return
        x > 0 ? 1.f :
        x < 0 ? -1.f :
        0.f;
}

__device__ void pack_ray_info(float3 origin, float3 direction)
{
    unsigned int offset = atomicAdd(&traverse_backup_buffer[0], 1U);
    offset *= 9;

    traverse_backup_buffer[1 + offset] = __float_as_uint(origin.x);
    traverse_backup_buffer[2 + offset] = __float_as_uint(origin.y);
    traverse_backup_buffer[3 + offset] = __float_as_uint(origin.z);

    traverse_backup_buffer[4 + offset] = __float_as_uint(direction.x);
    traverse_backup_buffer[5 + offset] = __float_as_uint(direction.y);
    traverse_backup_buffer[6 + offset] = __float_as_uint(direction.z);

    traverse_backup_buffer[7 + offset] = index.x;
    traverse_backup_buffer[8 + offset] = index.y;
    traverse_backup_buffer[9 + offset] = index.z;
}

__device__ float3 extract_direction_from_angles(float2 inclination_and_azimuthal_angles)
{
    float z = cosf(inclination_and_azimuthal_angles.x);
    float aux = sinf(inclination_and_azimuthal_angles.x);
    float x = aux*cosf(inclination_and_azimuthal_angles.y);
    float y = aux*sinf(inclination_and_azimuthal_angles.y);
    return make_float3(x, y, z);
}

__device__ float2 extract_angles_from_direction(float3 direction)
{
    float inclination_angle = acosf(direction.z);
    float aux{ direction.x / sqrtf(direction.x*direction.x + direction.y*direction.y) };
    float c = direction.y >= 0 ? 1.f : -1.f;
    float azimuthal_angle = acosf(aux*c) + M_PIf*static_cast<float>(direction.y < 0);

    return make_float2(inclination_angle, azimuthal_angle);
}

RT_PROGRAM void __ox_intersect__(void)
{
    int const dS = sign(-dot(normal, current_ray.direction));
    ray_payload.aux0 = MAX(0, static_cast<int>(ray_payload.aux0) + dS);
    float3 const p{ current_ray.origin + intersection_distance*current_ray.direction };
    unsigned int const ns{ MIN(OX_MAX_SPECTRA_PAIRS_SUPPORTED, num_spectra_supported) };

    if (dS > 0)    // the ray has entered object
    {
        unsigned int tracing_depth = ray_payload.tracing_depth + 1;

        if (tracing_depth <= max_recursion_depth)
        {
            OxRayRadiancePayload new_ray_payload{ ray_payload };
            new_ray_payload.tracing_depth = tracing_depth;
            new_ray_payload.aux = static_cast<unsigned int>(scattering_state);

            Ray new_ray = make_Ray(
                p + __uint_as_float(0x800000)*current_ray.direction,
                current_ray.direction,
                static_cast<unsigned int>(OxRayType::unknown), 0.f, step_size);

            rtTrace(ox_entry_node, new_ray, new_ray_payload);
            memcpy(ray_payload.spectral_radiant_exitance, new_ray_payload.spectral_radiant_exitance, sizeof(float2)*ns);
        }
        else
        {
            pack_ray_info(current_ray.origin, current_ray.direction);
        }
    }
}

RT_PROGRAM void __ox_intersect_scattered__(void)
{

}

RT_PROGRAM void __ox_miss__(void)
{
    if (ray_payload.trace_depth <= max_recursion_depth && ray_payload.aux > 0)
    {
        unsigned int const ns{ MIN(OX_MAX_SPECTRA_PAIRS_SUPPORTED, num_spectra_supported) };
        float3 p{ current_ray.origin + intersection_distance*current_ray.direction };
        float3 p_2{ current_ray.origin + intersection_distance / 2.f*current_ray.direction };
       
        // Compute scattering component when scattering is enabled
        if (num_importance_directions)
        {
            float2 direction_of_interest = extract_angles_from_direction(current_ray.direction);
            float2 S[OX_MAX_SPECTRA_PAIRS_SUPPORTED] = {};
            for (unsigned int j = 0U; j < num_importance_directions; ++j)
            {
                OxRayRadiancePayload scattered_payload{ ray_payload };
                ++scattered_payload.trace_depth;

                float2 importance_direction = importance_directions_buffer[j];
                Ray scattered_ray = make_Ray(
                    p,
                    extract_direction_from_angles(importance_direction),
                    static_cast<unsigned int>(OxRayType::scattered), 0.f, step_size
                );

                rtTrace(ox_entry_node, scattered_ray, scattered_payload);
                for (unsigned int k = 0; k < ns; ++k)
                    S[k] += scattered_payload.spectral_radiand_exitance[k] * phase_function(p, importance_direction, direction_of_interest) * sin(importance_direction.x);
            }
        }

        for (unsigned int i = 0U; i < ns; ++i)
        {
            float2 sigma_A = absorption_factor(p_2, i);
            float2 sigma_S = num_importance_directions ? scattering_factor(p_2, i) : make_float2(0.f, 0.f);

            float2 tau = expf(-(sigma_A + sigma_S)*intersection_distance);
            
            
        }
    }
}