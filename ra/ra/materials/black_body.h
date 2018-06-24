#ifndef OX_WRAPPER_MATERIALS_BLACK_BODY_H
#define OX_WRAPPER_MATERIALS_BLACK_BODY_H

#include "ra/materials/ra_materials_fwd.h"
#include "ra/ray_payloads.h"
#include "ra/material.h"

namespace ra { namespace materials {

class OxBlackBody : public OxMaterial
{
public:
    OxBlackBody(OxContext const& context, OxRayPayloadType payload_type, OxRayTypeCollection affected_ray_types);
};

}}

#endif
