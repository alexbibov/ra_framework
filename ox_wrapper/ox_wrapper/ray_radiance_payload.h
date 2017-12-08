#ifndef OX_WRAPPER_RAY_RADIANCE_PAYLOAD_H
#define OX_WRAPPER_RAY_RADIANCE_PAYLOAD_H

#include "commons.h"
#include "optixu/optixu_vector_types.h"

namespace ox_wrapper {

struct OxRayRadiancePayload final
{
    optix::float4 radiant_exitance[OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED];
    unsigned int tracing_depth;
    unsigned int flags;
};


enum class OxRayType : unsigned int
{
    unknown = 0U,
    shadow = 1U,
    scattered = 2U,
    reflected = 3U
};

}

#endif
