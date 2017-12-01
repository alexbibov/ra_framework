#include "context.h"
#include "geometry.h"

using namespace ox_wrapper;

OxGeometry::OxGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader, OxMaterialAssembly const& material_assembly):
    OxContractWithOxContext{ intersection_shader.context() },
    OxContractWithOxPrograms{ intersection_shader, aabb_shader },
    m_material_assembly{ material_assembly }
{
    RTgeometry native_geometry_handle{ nullptr };
    throwOptiXContextError(rtGeometryCreate(nativeOptiXContextHandle(), &native_geometry_handle));
    m_native_geometry.reset(native_geometry_handle, 
        [this](RTgeometry g)->void
    {
        logOptiXContextError(rtGeometryDestroy(g));
    });
    throwOptiXContextError(rtGeometrySetBoundingBoxProgram(native_geometry_handle, nativeOptiXProgramHandle(0U)));
    throwOptiXContextError(rtGeometrySetIntersectionProgram(native_geometry_handle, nativeOptiXProgramHandle(1U)));
    OxMaterialAssemblyAttorney<OxGeometry>::attachMaterialAssemblyToNativeGeometryHandle(material_assembly, m_native_geometry.get());
}

OxMaterialAssembly OxGeometry::getMaterialAssembly() const
{
    return m_material_assembly;
}

OxProgram OxGeometry::getAABBShader() const
{
    return getOxProgramFromDeclarationOffset(0U);
}

OxProgram OxGeometry::getIntersectionShader() const
{
    return getOxProgramFromDeclarationOffset(1U);
}

bool OxGeometry::isValid() const
{
    RTresult res = rtGeometryValidate(m_native_geometry.get());
    logOptiXContextError(res);
    return res == RT_SUCCESS && m_material_assembly.isValid();
}

void OxGeometry::setPrimitiveCount(unsigned int num_primitives)
{
    throwOptiXContextError(rtGeometrySetPrimitiveCount(m_native_geometry.get(), num_primitives));
}

unsigned int OxGeometry::getPrimitiveCount() const
{
    unsigned int num_primitives{};
    throwOptiXContextError(rtGeometryGetPrimitiveCount(m_native_geometry.get(), &num_primitives));
    return num_primitives;
}
