#ifndef RA_SHAPES_CIRCLE_H
#define RA_SHAPES_CIRCLE_H

#include "geometry.h"

namespace ra { namespace shapes {

class RaCircle : public RaGeometry
{
public:
    RaCircle(
        RaContext const& context,
        float location_x = 0.f,
        float location_y = 0.f,
        float radius = 1.f);

    RaCircle(
        RaContext const& context,
        RaMaterialAssembly const& material_assembly,
        float location_x = 0.f,
        float location_y = 0.f,
        float radius = 1.f);

    void setLocation(float2 const& new_location);
    void setLocation(float new_location_x, float new_location_y);
    void setRadius(float new_radius);

    float2 location() const;
    float radius() const;
};

}}

#endif
