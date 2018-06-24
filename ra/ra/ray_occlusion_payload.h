#ifndef OX_WRAPPER_RAY_OCCLUSION_PAYLOAD_H
#define OX_WRAPPER_RAY_OCCLUSION_PAYLOAD_H

#include "optix.h"

namespace ra {

struct RaRayOcclusionPayload final
{
    unsigned int is_occluded;
    unsigned int tracing_depth;
    optix::float2 depth;    // minimal and maximal intersection distances of current ray hit (particular meaning may vary depending on material)
};

}

#endif
