#ifndef RA_SHAPES_FACTORY_H
#define RA_SHAPER_FACTORY_H

#include "ra/context.h"

#include "circle.h"
#include "sphere.h"

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
    RaCircle createCircle(float location_x = 0.f, float location_y = 0.f, float radius = 1.f) const;
    RaCircle createCircle(RaMaterialAssembly const& material_assembly, float location_x = 0.f, float location_y = 0.f, float radius = 1.f) const;

    // RaSphere
public:
    RaSphere createSphere(float location_x = 0.f, float location_y = 0.f, float location_z = 0.f, float radius = 1.f) const;
    RaSphere createSphere(RaMaterialAssembly const& material_assembly, 
        float location_x = 0.f, float location_y = 0.f, float location_z = 0.f, float radius = 1.f) const;
    RaSphere createSphere(float3 const& location, float radius = 1.f) const;
    RaSphere createSphere(RaMaterialAssembly const& material_assembly, float3 const& location, float radius = 1.f) const;


private:
    RaShapesFactory(RaContext const& context);

private:
    RaContext const& m_context;
};

}}

#endif
