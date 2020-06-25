#ifndef RA_RAY_RADIANCE_PAYLOAD_H
#define RA_RAY_RADIANCE_PAYLOAD_H

#include "constants.h"
#include "optixu/optixu_vector_types.h"

namespace ra {

struct RaRayRadiancePayload final
{
    optix::float2 spectral_radiance[constants::max_spectra_pairs_supported];
    optix::float2 depth;    // minimal and maximal intersection distances of ray hit (particular meaning may vary depending on material)
    optix::uint4 tracing_depth_and_aux;    // depth of tracing recursion (x-component) and three auxiliary parameters (y-, z-, and w- components)
};

struct RaRayRadiancePayloadSimple final
{
    optix::float2 spectral_radiance;
    optix::float2 depth;    // minimal and maximal intersection distances of ray hit (particular meaning may vary depending on material)
    optix::uint4 tracing_depth_and_aux;    // depth of tracing recursion (x-component) and three auxiliary parameters (y-, z-, and w- components)
};

struct RaRayRadiancePayloadMonochromatic final
{
    float spectral_radiance;
    optix::float2 depth;    // minimal and maximal intersection distances of ray hit (particular meaning may vary depending on material)
    optix::uint4 tracing_depth_and_aux;    // depth of tracing recursion (x-component) and three auxiliary parameters (y-, z-, and w- components)
};

}

#endif
