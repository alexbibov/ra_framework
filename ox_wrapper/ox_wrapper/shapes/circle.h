#ifndef OX_WRAPPER_SHAPES_CIRCLE_H
#define OX_WRAPPER_SHAPES_CIRCLE_H

#include "../geometry.h"

namespace ox_wrapper { namespace shapes {

class OxCircle : public OxGeometry
{
public:
    OxCircle(
        OxContext const& context,
        OxMaterialAssembly const& material_assembly,
        float position_x = 0.f,
        float position_y = 0.f,
        float radius = 1.f);

    void updatePosition(float new_position_x, float new_position_y);
    void updateRadius(float new_radius);



private:
    float m_center_x, m_center_y;
    float m_radius;
};

}}

#endif
