#include "circle.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ox_wrapper::shapes;
using namespace ox_wrapper;

OxCircle::OxCircle(OxContext const& context, OxMaterialAssembly const& material_assembly,
    float position_x, float position_y, float radius):
    OxGeometry{ 
    context.createProgram(PTX_CIRCLE, OxProgram::Source::file, OX_SHADER_ENTRY_INTERSECTION),
    context.createProgram(PTX_CIRCLE, OxProgram::Source::file, OX_SHADER_ENTRY_BOUNDING_BOX),
    material_assembly }
{
    updatePosition(position_x, position_y);
    updateRadius(radius);
}

void shapes::OxCircle::updatePosition(float2 const& new_position)
{
    updatePosition(new_position.x, new_position.y);
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

float2 shapes::OxCircle::getPosition() const
{
    float2 rv{};
    getIntersectionShader().getVariableValue("center", &rv);
    return rv;
}

float shapes::OxCircle::getRadius() const
{
    float rv{};
    getIntersectionShader().getVariableValue("radius", &rv);
    return rv;
}