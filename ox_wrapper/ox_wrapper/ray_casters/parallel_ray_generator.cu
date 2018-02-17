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

__device__ optix::float2 rotate_point_on_X_axis(float point_to_rotate, float center_of_rotation, optix::float2 cs)
{
    optix::float2 rv = optix::make_float2(
        cs.x * point_to_rotate/*.x - cs.y * point_to_rotate.y*/ + (1 - cs.x)*center_of_rotation/*.x + cs.y*center_of_rotation.y*/,
        cs.y * point_to_rotate/*.x + cs.x * point_to_rotate.y*/ - cs.y*center_of_rotation/*.x + (1 - cs.x)*center_of_rotation.y*/
    );

    return rv;
}

RT_PROGRAM void __ox_generate__(void)
{
    optix::float3 origin{ -emitter_size / 2.f + emitter_size / (num_rays - 1) * index + emitter_position, 0.f, 0.f };
    //float3 direction{ 0.f, 1.f, 0.f };

    optix::float2 cs = optix::make_float2(cosf(emitter_rotation), sinf(emitter_rotation));
    optix::float2 rotated_point = rotate_point_on_X_axis(origin.x, emitter_position, cs);
    origin.x = rotated_point.x; origin.y = rotated_point.y;
    optix::float3 direction{ -cs.y, cs.x };
    optix::Ray ray = optix::make_Ray(origin, direction, static_cast<unsigned int>(ox_wrapper::OxRayType::unknown), 0.f, RT_DEFAULT_MAX);
    
    unsigned int const ns{ MIN(ox_wrapper::constants::max_spectra_pairs_supported, num_spectra_pairs_supported) };

    ox_wrapper::OxRayRadiancePayload payload{};
    memcpy(payload.spectral_radiance, &ox_init_flux_buffer[ns*index], sizeof(optix::float2)*ns);
    payload.tracing_depth_and_aux = make_uint4(0U, 0U, 0U, 0U);

    rtTrace(ox_entry_node, ray, payload);

    ox_output_buffer[index] = payload;
}