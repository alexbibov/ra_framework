#ifndef RA_SHAPES_SPHERE_H
#define RA_SHAPES_SPHERE_H

#include "ra/ra_fwd.h"
#include "ra/geometry.h"

namespace ra { namespace shapes {

class RaSphere : public RaGeometry
{
public:

    RaSphere(RaContext const& context, float3 const& location, float radius);

    RaSphere(RaContext const& context, RaMaterialAssembly const& material_assembly,
        float3 const& location, float radius);

    void setLocation(float3 const& new_location);
    void setLocation(float x, float y, float z);
    void setRadius(float radius);

    float3 location() const;
    float radius() const;
};

}}

#endif
