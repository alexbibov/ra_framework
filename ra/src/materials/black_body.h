#ifndef RA_MATERIALS_BLACK_BODY_H
#define RA_MATERIALS_BLACK_BODY_H

#include "materials/ra_materials_fwd.h"
#include "ray_payloads.h"
#include "material.h"

namespace ra { namespace materials {

class RaBlackBody : public RaMaterial
{
public:
    RaBlackBody(RaContext const& context, RaRayPayloadType payload_type, RaRayTypeCollection affected_ray_types);
};

}}

#endif
