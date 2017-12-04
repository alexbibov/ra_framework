#include "circle.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ox_wrapper::shapes;
using namespace ox_wrapper;

OxCircle::OxCircle(OxContext const& context, OxMaterialAssembly const& material_assembly,
    float position_x, float position_y, float radius):
    OxGeometry{ 
    context.createProgram(PTX_CIRCLE, OxProgram::Source::file, "intersect"),
    context.createProgram(PTX_CIRCLE, OxProgram::Source::file, "aabb"),
    material_assembly }
{
    updatePosition(position_x, position_y);
    updateRadius(radius);
}

void OxCircle::updatePosition(float new_position_x, float new_position_y)
{
    float2 pos{ new_position_x, new_position_y };
    getIntersectionShader().setVariableValue("center", pos);
    getAABBShader().setVariableValue("center", pos);
    markDirty();
}

void OxCircle::updateRadius(float new_radius)
{
    getIntersectionShader().setVariableValue("radius", new_radius);
    getAABBShader().setVariableValue("radius", new_radius);
    markDirty();
}
