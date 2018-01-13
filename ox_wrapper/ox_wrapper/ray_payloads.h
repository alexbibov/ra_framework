#ifndef OX_RAY_PAYLOADS
#define OX_RAY_PAYLOADS

namespace ox_wrapper {

enum class OxRayPayloadType : unsigned int
{
    radiance = 0,
    radiance_simple = 1,
    monochromatic = 2,
    occlusion = 3
};

enum class OxRayType : unsigned int
{
    unknown = 0U,
    shadow = 1U,
    scattered = 2U,
    reflected = 3U,
    count
};

}

#include "ray_radiance_payload.h"
#include "ray_occlusion_payload.h"

#endif
