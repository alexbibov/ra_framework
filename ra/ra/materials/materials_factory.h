#ifndef RA_MATERIALS_MATERIALS_FACTORY_H
#define RA_MATERIALS_MATERIALS_FACTORY_H

#include "ra/context.h"
#include "ra/materials/black_body.h"
#include "ra/ra_fwd.h"

namespace ra { namespace materials {

class RaMaterialsFactory final : public RaEntity
{
public:
    static RaMaterialsFactory* initialize(RaContext const& context);
    static void shutdown();
    static RaMaterialsFactory* retrieve();

public:
    // required by RaEntity interface
    bool isValid() const override;

    // RaBlackBody
public:
    RaBlackBody createBlackBody(RaRayPayloadType payload_type, RaRayTypeCollection const& affected_ray_types) const;

private:
    RaMaterialsFactory(RaContext const& context);

private:
    RaContext const& m_context;
};

}}

#endif

