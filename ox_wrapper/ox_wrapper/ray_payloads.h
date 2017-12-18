#ifndef OX_RAY_PAYLOADS
#define OX_RAY_PAYLOADS

enum class OxRayType : unsigned int
{
    unknown = 0U,
    shadow = 1U,
    scattered = 2U,
    reflected = 3U
};

#include "ray_radiance_payload.h"

#endif
