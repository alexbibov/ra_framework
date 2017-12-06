#ifndef OX_WRAPPER_RAY_PAYLOAD_H
#define OX_WRAPPER_RAY_PAYLOAD_H

#include "commons.h"
#include "optixu/optixu_vector_types.h"

namespace ox_wrapper {

struct OxRayPayload final
{
    optix::float4 radiant_exitance[OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED];
    unsigned int tracing_depth;
    unsigned int flags;
};

}

#define OX_RAY_TYPE_DEFAULT    0
#define OX_RAY_TYPE_SHADOW     1
#define OX_RAY_TYPE_SCATTERING 2

#endif
