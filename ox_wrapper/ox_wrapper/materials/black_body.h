#ifndef OX_WRAPPER_MATERIALS_BLACK_BODY_H
#define OX_WRAPPER_MATERIALS_BLACK_BODY_H

#include "../fwd.h"
#include "../ray_payloads.h"
#include "../material.h"

namespace ox_wrapper { namespace materials {

class OxBlackBody : public OxMaterial
{
public:
    OxBlackBody(OxContext const& context, OxRayPayloadType payload_type, OxRayType affected_ray_type);
};

}}

#endif
