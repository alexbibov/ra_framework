#ifndef OX_WRAPPER_MATERIALS_MATERIALS_FACTORY_H
#define OX_WRAPPER_MATERIALS_MATERIALS_FACTORY_H

#include "ra/context.h"
#include "ra/materials/black_body.h"
#include "ra/ra_fwd.h"

namespace ra { namespace materials {

class OxMaterialsFactory final : public OxEntity
{
public:
    static OxMaterialsFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxMaterialsFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

    // OxBlackBody
public:
    OxBlackBody createBlackBody(OxRayPayloadType payload_type, OxRayTypeCollection const& affected_ray_types) const;

private:
    OxMaterialsFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}}

#endif

