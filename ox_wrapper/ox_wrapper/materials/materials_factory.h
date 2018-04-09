#ifndef OX_WRAPPER_MATERIALS_MATERIALS_FACTORY_H
#define OX_WRAPPER_MATERIALS_MATERIALS_FACTORY_H

#include "ox_wrapper/context.h"
#include "ox_wrapper/materials/black_body.h"
#include "ox_wrapper/ox_wrapper_fwd.h"

namespace ox_wrapper { namespace materials {

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

