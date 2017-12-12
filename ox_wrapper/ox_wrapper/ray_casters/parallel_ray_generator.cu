#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/ray_radiance_payload.h"
#include "ox_wrapper/commons.h"


rtDeclareVariable(rtObject, ox_entry_node, , "Scene entry node");

rtDeclareVariable(unsigned int, num_rays, , "Number of casted rays");
rtDeclareVariable(float, emitter_position, , "Position of the emitter");
rtDeclareVariable(float, emitter_size, , "Size of the emitter");
rtDeclareVariable(float, emitter_rotation, , "Rotation of the emitter");
rtDeclareVariable(unsigned int, num_spectra_supported, , "Number of wavelengths in use");

rtDeclareVariable(unsigned int, index, rtLaunchIndex, "Thread index");

rtBuffer<ox_wrapper::OxRayRadiancePayload, 1> ox_output_buffer;

/*! The buffer is organized as follows:
 for each element of the emitter the buffer must contain M = MIN(OX_MAX_SPECTRA_PAIRS_SUPPORTED, num_spectra_supported)
 float2-elements, where each component (x and y) of each of these elements 
 defines spectral radiant exitance of the corresponding part of the spectrum. 
 All these values together therefore determine radiant exitance of single emission element of the emitter, 
 and the whole buffer determines the corresponding radiant flux
*/
rtBuffer<optix::float2, 1> ox_init_flux_buffer;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

RT_PROGRAM void __ox_generate__(void)
{
    optix::float3 origin{ -emitter_size / 2.f + emitter_size / (num_rays - 1) * index, 0.f, 0.f };
    //float3 direction{ 0.f, 1.f, 0.f };

    float c{ cosf(emitter_rotation) }, s{ sinf(emitter_rotation) };
    origin.x = c * origin.x - s * origin.y;
    origin.y = s * origin.x + c * origin.y;

    optix::float3 direction{ -s, c };

    optix::Ray ray = optix::make_Ray(origin, direction, static_cast<unsigned int>(ox_wrapper::OxRayType::unknown), 0.f, RT_DEFAULT_MAX);
    
    unsigned int const ns{ MIN(OX_MAX_SPECTRA_PAIRS_SUPPORTED, num_spectra_supported) };

    ox_wrapper::OxRayRadiancePayload payload{};
    memcpy(payload.spectral_radiant_exitance, &ox_init_buffer[ns*index], sizeof(optix::float2)*ns);
    payload.tracing_depth = 0U;
    payload.aux0 = payload.aux1 = payload.aux2 = 0U;

    rtTrace(ox_entry_node, ray, payload);

    ox_output_buffer[index] = payload;
}