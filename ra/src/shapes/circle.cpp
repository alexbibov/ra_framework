#include "circle.h"
#include "ptx.h"
#include "context.h"

using namespace ra::shapes;
using namespace ra;

RaCircle::RaCircle(RaContext const& context, float location_x, float location_y, float radius):
    RaGeometry{
    context.createProgram(PTX_CIRCLE, RaProgram::Source::file, OX_SHADER_ENTRY_INTERSECTION),
    context.createProgram(PTX_CIRCLE, RaProgram::Source::file, OX_SHADER_ENTRY_BOUNDING_BOX)
    }
{
    setPrimitiveCount(1U);
    setLocation(location_x, location_y);
    setRadius(radius);
}

RaCircle::RaCircle(RaContext const& context, RaMaterialAssembly const& material_assembly,
    float location_x, float location_y, float radius):
    RaCircle{ context, location_x, location_y, radius }
{
    setPrimitiveCount(1U);
    setMaterialAssembly(material_assembly);
}

void shapes::RaCircle::setLocation(float2 const& new_location)
{
    setLocation(new_location.x, new_location.y);
}

void RaCircle::setLocation(float new_location_x, float new_location_y)
{
    float2 pos{ new_location_x, new_location_y };
    getIntersectionShader().setVariableValue("location", pos);
    getAABBShader().setVariableValue("location", pos);
    markDirty();
}

void RaCircle::setRadius(float new_radius)
{
    getIntersectionShader().setVariableValue("radius", new_radius);
    getAABBShader().setVariableValue("radius", new_radius);
    markDirty();
}

float2 shapes::RaCircle::location() const
{
    float2 rv{};
    getIntersectionShader().getVariableValue("location", rv);
    return rv;
}

float shapes::RaCircle::radius() const
{
    float rv{};
    getIntersectionShader().getVariableValue("radius", rv);
    return rv;
}
