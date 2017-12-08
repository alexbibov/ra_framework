#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/ray_radiance_payload.h"
#include "ox_wrapper/commons.h"


rtDeclareVariable(unsigned int, num_rays, , "Number of casted rays");
rtDeclareVariable(float, emitter_position, , "Position of the emitter");
rtDeclareVariable(float, emitter_size, , "Size of the emitter");
rtDeclareVariable(float, emitter_rotation, , "Rotation of the emitter");
rtDeclareVariable(rtObject, ox_entry_node, , "Scene entry node");

rtDeclareVariable(optix::uint1, index, rtLaunchIndex, "Thread index");

rtBuffer<ox_wrapper::OxRayRadiancePayload, 1> ox_output_buffer;

/*! This buffer must be organized as follows: 
 for each generated ray the buffer stores 4*OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED values
 each determining spectral exitance for the corresponding part of the spectrum. All 4*OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED
 elements therefore define the radiant exitance and the buffer in total determines the radiant flux of the emitter.
*/
rtBuffer<float, 1> ox_init_spectral_flux_buffer;

RT_PROGRAM void __ox_generate__(void)
{
    optix::float3 origin{ -emitter_size / 2.f + emitter_size / (num_rays - 1) * index.x, 0.f, 0.f };
    //float3 direction{ 0.f, 1.f, 0.f };

    float c{ cosf(emitter_rotation) }, s{ sinf(emitter_rotation) };
    origin.x = c * origin.x - s * origin.y;
    origin.y = s * origin.x + c * origin.y;

    optix::float3 direction{ -s, c };

    optix::Ray ray = optix::make_Ray(origin, direction, static_cast<unsigned int>(ox_wrapper::OxRayType::unknown), 0.f, RT_DEFAULT_MAX);


    optix::float4 radiant_exitance[OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED];
    for (unsigned int i = 0; i < OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED; ++i)
    {
        radiant_exitance[i].x = ox_init_spectral_flux_buffer[4 * OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED*index.x + 0];
        radiant_exitance[i].y = ox_init_spectral_flux_buffer[4 * OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED*index.x + 1];
        radiant_exitance[i].z = ox_init_spectral_flux_buffer[4 * OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED*index.x + 2];
        radiant_exitance[i].w = ox_init_spectral_flux_buffer[4 * OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED*index.x + 3];
    }

    ox_wrapper::OxRayRadiancePayload payload{};
    memcpy(payload.radiant_exitance, radiant_exitance, sizeof(optix::float4)*OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED);
    payload.tracing_depth = 0U;
    payload.flags = 0U;

    rtTrace(ox_entry_node, ray, payload);
}