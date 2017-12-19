#ifndef OX_WRAPPER_RAY_OCCLUSION_PAYLOAD_H
#define OX_WRAPPER_RAY_OCCLUSION_PAYLOAD_H

namespace ox_wrapper {

struct OxRayOcclusionPayload final
{
    unsigned int is_occluded;
    unsigned int tracing_depth;
};

}

#endif
