#ifndef OX_RAY_PAYLOADS
#define OX_RAY_PAYLOADS

namespace ox_wrapper {

enum class OxRayType : unsigned int
{
    unknown = 0U,
    shadow = 1U,
    scattered = 2U,
    reflected = 3U
};

}

#include "ray_radiance_payload.h"
#include "ray_occlusion_payload.h"

#endif
