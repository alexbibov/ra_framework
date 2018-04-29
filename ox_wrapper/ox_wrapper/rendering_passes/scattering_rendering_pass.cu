#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/constants.h"
#include "ox_wrapper/ray_payloads.h"


rtDeclareVariable(rtObject, ox_entry_node, , "Scene entry node");

rtDeclareVariable(float, step_size, , "Ray marching step size");
rtDeclareVariable(uint3, problem_size, , "Original size of the problem");
rtDeclareVariable(optix::uint, max_recursion_depth, , "Maximal depth of recursion for scattering traverse");
rtDeclareVariable(optix::uint, num_spectra_pairs_supported, , "Number of wavelengths in use");
rtDeclareVariable(optix::uint, num_importance_directions, , );

typedef rtCallableProgramId<optix::float2(optix::float3, unsigned int)> absorption_factor_program_id_type;
typedef rtCallableProgramId<optix::float2(optix::float3, unsigned int)> scattering_factor_program_id_type;
typedef rtCallableProgramId<optix::float2(optix::float3, optix::float2, optix::float2, unsigned int)> phase_function_program_id_type; 
rtDeclareVariable(absorption_factor_program_id_type, absorption_factor, , );
rtDeclareVariable(scattering_factor_program_id_type, scattering_factor, , );
rtDeclareVariable(phase_function_program_id_type, phase_function, , );

rtDeclareVariable(ox_wrapper::OxRayRadiancePayload, ray_payload, rtPayload, "Current ray payload");
rtDeclareVariable(ox_wrapper::OxRayRadiancePayloadSimple, ray_payload_scattered, rtPayload, "Payload of the current scattering ray");
rtDeclareVariable(float, intersection_distance, rtIntersectionDistance, "Parametric distance from ray origin to the intersection");
rtDeclareVariable(optix::Ray, current_ray, rtCurrentRay, "Currently traversed ray");
rtDeclareVariable(optix::uint3, index, rtLaunchIndex, "Index of the current ray");

rtDeclareVariable(optix::float3, normal, attribute attrNormal, "Normal of the surface being hit");

/*! Determines importance sampling domain for in-scattering component of the volumetric light
 transport equation. The buffer consists of a number of importance direction records with layout
 of each record defined as follows:
 — record begins by a float2 element that defines spherical coordinates of importance direction vector,
 more precisely float2.x stores inclination and float2.y stores azimuthal angle
 — the following are N more float2 elements, where each component (i.e. x- and y-) identifies single spectral
 radiance value in the given importance direction OUTSIDE of medium (therefore, current design only supports
 cases where spectral radiance outside of the medium does not depend on spatial location).
 
 *Here N=min(num_spectra_pairs_supported, constants::max_spectra_pairs_supported)
*/
rtBuffer<optix::float2, 1> importance_directions_buffer;

/*! Traverse backup is a standard buffer, which enables implementation of multi-pass ray-tracing algorithms.
 The buffer has to be organized as follows:
 1st element contains number of vectors stored in the buffer, hereinafter denoted by N
 The rest of the buffer must contain at least 6*N floats and 3*N unsigned integers ordered in 9-element
 tuples with the first 3 elements being floating point numbers determining the origin of the 
 corresponding ray, succeeding 3 elements being floating point components of the ray's direction, and
 the last 3 unsigned integers determining three-dimensional index of the ray in its original launch grid
*/
rtBuffer<unsigned int, 1> traverse_backup_buffer;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

using namespace optix;
using namespace ox_wrapper;

__device__ uint3 linear_index_to_3d_index(unsigned int idx)
{
    unsigned int layer_idx = idx / (problem_size.x*problem_size.y);
    unsigned int aux = idx - layer_idx * problem_size.x*problem_size.y;
    
    unsigned int scanline_idx = aux / problem_size.x;
    unsigned int ray_idx = aux - scanline_idx * problem_size.x;

    return optix::make_uint3(ray_idx, scanline_idx, layer_idx);
}

__device__ float sign(float x)
{
    return
        x > 0 ? 1.f :
        x < 0 ? -1.f :
        0.f;
}

__device__ void pack_ray_info(float3 origin, float3 direction, uint3 idx)
{
    unsigned int offset = atomicAdd(&traverse_backup_buffer[0], 1U);
    offset *= 9;

    traverse_backup_buffer[1 + offset] = __float_as_uint(origin.x);
    traverse_backup_buffer[2 + offset] = __float_as_uint(origin.y);
    traverse_backup_buffer[3 + offset] = __float_as_uint(origin.z);

    traverse_backup_buffer[4 + offset] = __float_as_uint(direction.x);
    traverse_backup_buffer[5 + offset] = __float_as_uint(direction.y);
    traverse_backup_buffer[6 + offset] = __float_as_uint(direction.z);

    traverse_backup_buffer[7 + offset] = idx.x;
    traverse_backup_buffer[8 + offset] = idx.y;
    traverse_backup_buffer[9 + offset] = idx.z;
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

__device__ void update_ray_payload(float3 p, float3 p_2, float2 direction_of_interest, float step)
{
    unsigned int const ns{ MIN(constants::max_spectra_pairs_supported, num_spectra_pairs_supported) };
    for (unsigned int i = 0U; i < ns; ++i)
    {
        float2 S = make_float2(0.f, 0.f);

        // scattering component is only calculated when scattering is enabled
        for (int j = 0; j < num_importance_directions; ++j)
        {
            OxRayRadiancePayloadSimple scattered_payload;
            scattered_payload.spectral_radiance = make_float2(0.f, 0.f);
            scattered_payload.tracing_depth_and_aux =
                make_uint4(
                    ray_payload.tracing_depth_and_aux.x + 1,
                    ray_payload.tracing_depth_and_aux.y,
                    j * (ns + 1) + 1 + i,
                    i);

            float2 importance_direction = importance_directions_buffer[j*(ns + 1)];
            Ray scattered_ray = make_Ray(
                p,
                extract_direction_from_angles(importance_direction),
                static_cast<unsigned int>(OxRayType::scattered), 0.f, step_size);

            rtTrace(ox_entry_node, scattered_ray, scattered_payload);

            S += scattered_payload.spectral_radiance
                * phase_function(p, importance_direction, direction_of_interest, i) * sin(importance_direction.x);
        }

        float2 sigma_S_p_2 = num_importance_directions ? scattering_factor(p_2, i) : make_float2(0.f, 0.f);
        float2 phi = expf(-(absorption_factor(p_2, i) + sigma_S_p_2)*step);

        ray_payload.spectral_radiance[i] =
            ray_payload.spectral_radiance[i] * phi
            + S * scattering_factor(p, i)*step;
    }
}

RT_PROGRAM void __ox_closest_hit__(void)
{
    int dS = static_cast<int>(sign(-dot(normal, current_ray.direction)));
    ray_payload.tracing_depth_and_aux.y = MAX(0, static_cast<int>(ray_payload.tracing_depth_and_aux.y) + dS);
    float3 p{ current_ray.origin + intersection_distance*current_ray.direction };

    
    if (dS > 0)    // the ray has entered object
    {
        ray_payload.depth.x = intersection_distance;
        ray_payload.depth.y = intersection_distance;

        ++ray_payload.tracing_depth_and_aux.x;
        if (ray_payload.tracing_depth_and_aux.x <= max_recursion_depth)
        {
            float const correction = step_size*1e-2f;

            Ray subsurface_ray = make_Ray(
                p + correction*current_ray.direction,
                current_ray.direction,
                static_cast<unsigned int>(OxRayType::unknown), 0.f, step_size + correction);

            rtTrace(ox_entry_node, subsurface_ray, ray_payload);
        }
        else
        {
            ray_payload.tracing_depth_and_aux.x = 0U;

            uint3 idx = ray_payload.tracing_depth_and_aux.z > 0 ?
                linear_index_to_3d_index(ray_payload.tracing_depth_and_aux.z - 1) : index;
            // ray_payload.tracing_depth_and_aux.z = 0U;

            pack_ray_info(current_ray.origin, current_ray.direction, idx);
        }
    }
    else if (dS < 0)    // the ray has left object
    {
        ray_payload.depth.y += intersection_distance;

        float3 p_2{ current_ray.origin + intersection_distance * .5f * current_ray.direction };
        float2 direction_of_interest = extract_angles_from_direction(current_ray.direction);

        update_ray_payload(p, p_2, direction_of_interest, intersection_distance);

        // we still need to try to keep traversing the ray as there might be more media to discover
        ++ray_payload.tracing_depth_and_aux.x;
        if (ray_payload.tracing_depth_and_aux.x <= max_recursion_depth)
        {
            float const correction = step_size * 1e-2f;

            Ray next_iteration_ray = make_Ray(
                p + correction*current_ray.direction,
                current_ray.direction,
                static_cast<unsigned int>(OxRayType::unknown), 0.f, RT_DEFAULT_MAX);

            rtTrace(ox_entry_node, next_iteration_ray, ray_payload);
        }
    }

    // ray has "touched" object: no operation is required
}

RT_PROGRAM void __ox_miss__(void)
{
    if (ray_payload.tracing_depth_and_aux.y >= 1)    // "miss" has happened inside of medium
    {
        float3 p{ current_ray.origin + step_size * current_ray.direction };
        float3 p_2{ current_ray.origin + step_size * .5f * current_ray.direction };
        float2 direction_of_interest = extract_angles_from_direction(current_ray.direction);

        ray_payload.depth.y += step_size;
        update_ray_payload(p, p_2, direction_of_interest, step_size);

        if (ray_payload.tracing_depth_and_aux.x <= max_recursion_depth)
        {
            ++ray_payload.tracing_depth_and_aux.x;

            Ray next_iteration_ray = make_Ray(
                p, current_ray.direction,
                static_cast<unsigned int>(OxRayType::unknown), 0.f, step_size);

            rtTrace(ox_entry_node, next_iteration_ray, ray_payload);
        }
        else
        {
            ray_payload.tracing_depth_and_aux.x = 0U;

            uint3 idx = ray_payload.tracing_depth_and_aux.z >= 1 ?
                /*optix::make_uint3(ray_payload.tracing_depth_and_aux.z - 1, 0, 0)*/
                linear_index_to_3d_index(ray_payload.tracing_depth_and_aux.z - 1) : index;
            // ray_payload.tracing_depth_and_aux.z = 0U;

            pack_ray_info(p, current_ray.direction, idx);
        }
    }

}

RT_PROGRAM void __ox_closest_hit_scattered__(void)
{
    // this shader is only called when scattered ray exits the domain of the medium
    unsigned int idb_offset = ray_payload_scattered.tracing_depth_and_aux.z;

    float2 incoming_spectral_radiance = importance_directions_buffer[idb_offset];
    ray_payload_scattered.spectral_radiance = incoming_spectral_radiance;
}

RT_PROGRAM void __ox_miss_scattered__(void)
{
    // this shader is only invoked from within the medium

    ++ray_payload_scattered.tracing_depth_and_aux.x;

    if (ray_payload_scattered.tracing_depth_and_aux.x <= max_recursion_depth)
    {
        float3 p{ current_ray.origin + step_size * current_ray.direction };
        float3 p_2{ current_ray.origin + step_size * .5f * current_ray.direction };

        Ray next_scattered_ray_iteration = make_Ray(
            p, current_ray.direction,
            static_cast<unsigned int>(OxRayType::scattered), 0.f, 
            ray_payload_scattered.tracing_depth_and_aux.x < max_recursion_depth ? step_size : RT_DEFAULT_MAX);
        rtTrace(ox_entry_node, next_scattered_ray_iteration, ray_payload_scattered);

        unsigned int spectrum = ray_payload_scattered.tracing_depth_and_aux.w;
        float2 phi = expf(-absorption_factor(p_2, spectrum) * step_size);
        ray_payload_scattered.spectral_radiance *= phi;
    }
}
