#ifndef OX_WRAPPER_RAY_RADIANCE_PAYLOAD_H
#define OX_WRAPPER_RAY_RADIANCE_PAYLOAD_H

#include "constants.h"
#include "optixu/optixu_vector_types.h"

namespace ox_wrapper {

struct OxRayRadiancePayload final
{
    optix::float2 spectral_radiance[constants::max_spectra_pairs_supported];
    unsigned int tracing_depth;
    unsigned int aux0;
    unsigned int aux1;
    unsigned int aux2;
};

}

#endif
