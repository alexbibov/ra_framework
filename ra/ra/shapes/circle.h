#ifndef OX_WRAPPER_SHAPES_CIRCLE_H
#define OX_WRAPPER_SHAPES_CIRCLE_H

#include "../geometry.h"

namespace ra { namespace shapes {

class RaCircle : public RaGeometry
{
public:
    RaCircle(
        RaContext const& context,
        float position_x = 0.f,
        float position_y = 0.f,
        float radius = 1.f);

    RaCircle(
        RaContext const& context,
        RaMaterialAssembly const& material_assembly,
        float position_x = 0.f,
        float position_y = 0.f,
        float radius = 1.f);

    void updatePosition(float2 const& new_position);
    void updatePosition(float new_position_x, float new_position_y);
    void updateRadius(float new_radius);

    float2 getPosition() const;
    float getRadius() const;

private:
    float m_center_x, m_center_y;
    float m_radius;
};

}}

#endif
