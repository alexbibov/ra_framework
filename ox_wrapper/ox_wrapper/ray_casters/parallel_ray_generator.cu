#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/ray_payloads.h"
#include "ox_wrapper/constants.h"


rtDeclareVariable(rtObject, ox_entry_node, , "Scene entry node");

rtDeclareVariable(unsigned int, num_rays, , "Number of casted rays");
rtDeclareVariable(float, emitter_position, , "Position of the emitter");
rtDeclareVariable(float, emitter_size, , "Size of the emitter");
rtDeclareVariable(float, emitter_rotation, , "Rotation of the emitter");
rtDeclareVariable(optix::uint, num_spectra_pairs_supported, , "Number of wavelengths in use");

rtDeclareVariable(unsigned int, index, rtLaunchIndex, "Thread index");

rtBuffer<ox_wrapper::OxRayRadiancePayload, 1> ox_output_buffer;

/*! The buffer is organized as follows:
 for each element of the emitter the buffer must contain M = MIN(constants::max_spectra_pairs_supported, num_spectra_pairs_supported)
 float2-elements, where each component (x and y) of each of these elements 
 defines spectral radiant exitance of the corresponding part of the spectrum. 
 All these values together therefore determine radiant exitance of single emission element of the emitter, 
 and the whole buffer determines the corresponding radiant flux
*/
rtBuffer<optix::float2, 1> ox_init_flux_buffer;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

__device__ optix::float2 rotate_point_2d(optix::float2 point_to_rotate, optix::float2 center_of_rotation, optix::float2 cs)
{
    point_to_rotate -= center_of_rotation;

    optix::float2 rv = optix::make_float2(
        cs.x * point_to_rotate.x - cs.y * point_to_rotate.y,
        cs.y * point_to_rotate.x + cs.x * point_to_rotate.y
    );

    rv += center_of_rotation;

    return rv;
}

RT_PROGRAM void __ox_generate__(void)
{
    optix::float3 origin{ -emitter_size / 2.f + emitter_size / (num_rays - 1) * index + emitter_position, 0.f, 0.f };

    optix::float2 cs = optix::make_float2(cosf(emitter_rotation), sinf(emitter_rotation));
    optix::float2 rotated_point = rotate_point_2d(optix::float2{ origin.x, origin.y }, optix::float2{ emitter_position, 0 }, cs);
    optix::float2 rotated_tip = rotate_point_2d(optix::float2{ origin.x, 1 }, optix::float2{ emitter_position, 0 }, cs);
    
    origin.x = rotated_point.x; origin.y = rotated_point.y;
    optix::float3 direction = optix::normalize(optix::make_float3(rotated_tip - rotated_point, 0));
    optix::Ray ray = optix::make_Ray(origin, direction, static_cast<unsigned int>(ox_wrapper::OxRayType::unknown), 0.f, RT_DEFAULT_MAX);
    
    unsigned int const ns{ MIN(ox_wrapper::constants::max_spectra_pairs_supported, num_spectra_pairs_supported) };

    ox_wrapper::OxRayRadiancePayload payload{};
    memcpy(payload.spectral_radiance, &ox_init_flux_buffer[ns*index], sizeof(optix::float2)*ns);
    payload.tracing_depth_and_aux = make_uint4(0U, 0U, 0U, 0U);
    rtTrace(ox_entry_node, ray, payload);

    ox_output_buffer[index] = payload;
}