#include "sphere.h"
#include "ptx.h"
#include "context.h"
#include "program.h"

using namespace ra;
using namespace ra::shapes;

RaSphere::RaSphere(RaContext const& context, float3 const& location, float radius):
    RaGeometry{
    context.createProgram(PTX_SPHERE, RaProgram::Source::file, OX_SHADER_ENTRY_INTERSECTION),
    context.createProgram(PTX_SPHERE, RaProgram::Source::file, OX_SHADER_ENTRY_BOUNDING_BOX)
    }
{
    setPrimitiveCount(1U);
    setLocation(location);
    setRadius(radius);
}

RaSphere::RaSphere(RaContext const& context, RaMaterialAssembly const& material_assembly, 
    float3 const& location, float radius):
    RaGeometry{
    context.createProgram(PTX_SPHERE, RaProgram::Source::file, OX_SHADER_ENTRY_INTERSECTION),
    context.createProgram(PTX_SPHERE, RaProgram::Source::file, OX_SHADER_ENTRY_BOUNDING_BOX),
    material_assembly
    }
{
    setPrimitiveCount(1U);
    setLocation(location);
    setRadius(radius);
}

void RaSphere::setLocation(float3 const& new_location)
{
    getIntersectionShader().setVariableValue("location", new_location);
    getAABBShader().setVariableValue("location", new_location);
    markDirty();
}

void RaSphere::setLocation(float x, float y, float z)
{
    setLocation(float3{ x, y, z });
}

void RaSphere::setRadius(float radius)
{
    getIntersectionShader().setVariableValue("radius", radius);
    getAABBShader().setVariableValue("radius", radius);
    markDirty();
}

float3 RaSphere::location() const
{
    float3 rv{};
    getIntersectionShader().getVariableValue("location", rv);
    return rv;
}

float RaSphere::radius() const
{
    float rv{};
    getIntersectionShader().getVariableValue("radius", rv);
    return rv;
}
