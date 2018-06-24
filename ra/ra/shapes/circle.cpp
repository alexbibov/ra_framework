#include "circle.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ra::shapes;
using namespace ra;

RaCircle::RaCircle(RaContext const& context, float position_x, float position_y, float radius):
    RaGeometry{
    context.createProgram(PTX_CIRCLE, RaProgram::Source::file, OX_SHADER_ENTRY_INTERSECTION),
    context.createProgram(PTX_CIRCLE, RaProgram::Source::file, OX_SHADER_ENTRY_BOUNDING_BOX)
    }
{
    setPrimitiveCount(1U);
    updatePosition(position_x, position_y);
    updateRadius(radius);
}

RaCircle::RaCircle(RaContext const& context, RaMaterialAssembly const& material_assembly,
    float position_x, float position_y, float radius):
    RaCircle{ context, position_x, position_y, radius }
{
    setPrimitiveCount(1U);
    setMaterialAssembly(material_assembly);
}

void shapes::RaCircle::updatePosition(float2 const& new_position)
{
    updatePosition(new_position.x, new_position.y);
}

void RaCircle::updatePosition(float new_position_x, float new_position_y)
{
    float2 pos{ new_position_x, new_position_y };
    getIntersectionShader().setVariableValue("center", pos);
    getAABBShader().setVariableValue("center", pos);
    markDirty();
}

void RaCircle::updateRadius(float new_radius)
{
    getIntersectionShader().setVariableValue("radius", new_radius);
    getAABBShader().setVariableValue("radius", new_radius);
    markDirty();
}

float2 shapes::RaCircle::getPosition() const
{
    float2 rv{};
    getIntersectionShader().getVariableValue("center", rv);
    return rv;
}

float shapes::RaCircle::getRadius() const
{
    float rv{};
    getIntersectionShader().getVariableValue("radius", rv);
    return rv;
}
