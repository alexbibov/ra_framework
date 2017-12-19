#ifndef OX_WRAPPER_RAY_RADIANCE_PAYLOAD_H
#define OX_WRAPPER_RAY_RADIANCE_PAYLOAD_H

#include "constants.h"
#include "optixu/optixu_vector_types.h"

namespace ox_wrapper {

struct OxRayRadiancePayload final
{
    optix::float2 spectral_radiance[constants::max_spectra_pairs_supported];
    optix::uint4 tracing_depth_and_aux;    // depth of tracing recursion (x-component) and three auxiliary parameters (y-, z-, and w- components)
};

struct OxRayRadiancePayloadSimple final
{
    optix::float2 spectral_radiance;
    optix::uint4 tracing_depth_and_aux;    // depth of tracing recursion (x-component) and three auxiliary parameters (y-, z-, and w- components)
};

struct OxRayRadiancePayloadMonochromatic final
{
    float spectral_radiance;
    optix::uint4 tracing_depth_and_aux;    // depth of tracing recursion (x-component) and three auxiliary parameters (y-, z-, and w- components)
};

}

#endif
