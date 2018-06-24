#ifndef OX_WRAPPER_SHAPES_FACTORY_H
#define OX_WRAPPER_SHAPER_FACTORY_H

#include "../context.h"

#include "circle.h"

namespace ra { namespace shapes {

class RaShapesFactory : public RaEntity
{
public:
    static RaShapesFactory* initialize(RaContext const& context);
    static void shutdown();
    static RaShapesFactory* retrieve();

public:
    // required by RaEntity interface
    bool isValid() const override;

    // RaCircle
public:
    RaCircle createCircle(float position_x = 0.f, float position_y = 0.f, float radius = 1.f) const;
    RaCircle createCircle(RaMaterialAssembly const& material_assembly, float position_x = 0.f, float position_y = 0.f, float radius = 1.f) const;

private:
    RaShapesFactory(RaContext const& context);

private:
    RaContext const& m_context;
};

}}

#endif
