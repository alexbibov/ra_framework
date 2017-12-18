#include "context.h"
#include "geometry.h"

using namespace ox_wrapper;

OxGeometry::OxGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader):
    OxContractWithOxContext{ intersection_shader.context() },
    OxContractWithOxPrograms{ intersection_shader, aabb_shader }
{
    RTgeometry native_geometry_handle{ nullptr };
    throwOptiXContextError(rtGeometryCreate(nativeOptiXContextHandle(), &native_geometry_handle));
    m_native_geometry.reset(new std::pair<RTgeometry, bool>{ native_geometry_handle, true },
        [this](std::pair<RTgeometry, bool>* e)->void
    {
        logOptiXContextError(rtGeometryDestroy(e->first));
    });

    throwOptiXContextError(rtGeometrySetBoundingBoxProgram(native_geometry_handle, nativeOptiXProgramHandle(0U)));
    throwOptiXContextError(rtGeometrySetIntersectionProgram(native_geometry_handle, nativeOptiXProgramHandle(1U)));
}

OxGeometry::OxGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader, OxMaterialAssembly const& material_assembly):
    OxGeometry{ intersection_shader, aabb_shader }
{
    setMaterialAssembly(material_assembly);
}

util::Optional<OxMaterialAssembly> OxGeometry::getMaterialAssembly() const
{
    return m_material_assembly;
}

void OxGeometry::setMaterialAssembly(OxMaterialAssembly const& material_assembly)
{
    m_material_assembly = material_assembly;
    OxMaterialAssemblyAttorney<OxGeometry>::attachMaterialAssemblyToNativeGeometryHandle(material_assembly, m_native_geometry->first);
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
    RTresult res = rtGeometryValidate(m_native_geometry->first);
    logOptiXContextError(res);
    return res == RT_SUCCESS && m_material_assembly.isValid();
}

void OxGeometry::setPrimitiveCount(unsigned int num_primitives)
{
    throwOptiXContextError(rtGeometrySetPrimitiveCount(m_native_geometry->first, num_primitives));
    markDirty();
}

void OxGeometry::markDirty()
{
    m_native_geometry->second = true;
}

void OxGeometry::update(OxObjectHandle top_scene_object) const
{
    OxMaterialAssemblyAttorney<OxGeometry>::updateMaterialAssembly(m_material_assembly, top_scene_object);
}

unsigned int OxGeometry::getPrimitiveCount() const
{
    unsigned int num_primitives{};
    throwOptiXContextError(rtGeometryGetPrimitiveCount(m_native_geometry->first, &num_primitives));
    return num_primitives;
}
