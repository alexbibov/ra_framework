#include "circle.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ox_wrapper::shapes;
using namespace ox_wrapper;

OxCircle::OxCircle(OxContext const& context, OxMaterialAssembly const& material_assembly,
    float position_x, float position_y, float position_z, float radius):
    OxGeometry{ 
    context.createProgram(PTX_CIRCLE, OxProgram::Source::file, "intersect"),
    context.createProgram(PTX_CIRCLE, OxProgram::Source::file, "aabb"),
    material_assembly },
    m_center_x{ 0.f }, 
    m_center_y{ 0.f },
    m_center_z{ 0.f },
    m_radius{ 1.f }
{
}
