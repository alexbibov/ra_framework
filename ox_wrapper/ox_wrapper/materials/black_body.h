#ifndef OX_WRAPPER_MATERIALS_BLACK_BODY_H
#define OX_WRAPPER_MATERIALS_BLACK_BODY_H

#include "ox_wrapper/materials/ox_wrapper_materials_fwd.h"
#include "ox_wrapper/ray_payloads.h"
#include "ox_wrapper/material.h"

namespace ox_wrapper { namespace materials {

class OxBlackBody : public OxMaterial
{
public:
    OxBlackBody(OxContext const& context, OxRayPayloadType payload_type, OxRayTypeCollection affected_ray_types);
};

}}

#endif
