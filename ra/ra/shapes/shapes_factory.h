#ifndef OX_WRAPPER_SHAPES_FACTORY_H
#define OX_WRAPPER_SHAPER_FACTORY_H

#include "../context.h"

#include "circle.h"

namespace ra { namespace shapes {

class OxShapesFactory : public OxEntity
{
public:
    static OxShapesFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxShapesFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

    // OxCircle
public:
    OxCircle createCircle(float position_x = 0.f, float position_y = 0.f, float radius = 1.f) const;
    OxCircle createCircle(OxMaterialAssembly const& material_assembly, float position_x = 0.f, float position_y = 0.f, float radius = 1.f) const;

private:
    OxShapesFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}}

#endif
