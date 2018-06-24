#include "context.h"
#include "geometry.h"

using namespace ra;

OxGeometry::OxGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader):
    OxContractWithOxContext{ intersection_shader.context() },
    OxContractWithOxPrograms{ intersection_shader, aabb_shader }
{
    RTgeometry native_geometry_handle{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryCreate(nativeOptiXContextHandle(), &native_geometry_handle));
    m_geometry_blueprint.reset(new geometry_blueprint{ native_geometry_handle, true, util::Optional<OxMaterialAssembly>{} },
        [this](geometry_blueprint* e)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryDestroy(e->native_handle));
        delete e;
    });

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometrySetIntersectionProgram(native_geometry_handle, nativeOptiXProgramHandle(0U)));
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometrySetBoundingBoxProgram(native_geometry_handle, nativeOptiXProgramHandle(1U)));
}

OxGeometry::OxGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader, OxMaterialAssembly const& material_assembly):
    OxGeometry{ intersection_shader, aabb_shader }
{
    setMaterialAssembly(material_assembly);
}

util::Optional<OxMaterialAssembly> OxGeometry::getMaterialAssembly() const
{
    return m_geometry_blueprint->material_assembly;
}

void OxGeometry::setMaterialAssembly(OxMaterialAssembly const& material_assembly) const
{
    m_geometry_blueprint->material_assembly = material_assembly;
    OxMaterialAssemblyAttorney<OxGeometry>::attachMaterialAssemblyToNativeGeometryHandle(material_assembly, m_geometry_blueprint->native_handle);
    m_geometry_blueprint->is_dirty = true;
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
    bool has_material_assembly_attached;
    if (!(has_material_assembly_attached = m_geometry_blueprint->material_assembly.isValid()))
    {
        util::Log::retrieve()->out("Geometry \"" + getStringName() + "\" does not have material assembly attached to it", 
            util::LogMessageType::exclamation);
        return false;
    }

    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtGeometryValidate(m_geometry_blueprint->native_handle));
    return res == RT_SUCCESS && has_material_assembly_attached 
        && static_cast<OxMaterialAssembly&>(m_geometry_blueprint->material_assembly).isValid();
}

void OxGeometry::setPrimitiveCount(unsigned int num_primitives)
{
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometrySetPrimitiveCount(m_geometry_blueprint->native_handle, num_primitives));
    markDirty();
}

void OxGeometry::markDirty()
{
    m_geometry_blueprint->is_dirty = true;
}

void OxGeometry::update(OxObjectHandle top_scene_object) const
{
    if (m_geometry_blueprint->material_assembly.isValid()
        && static_cast<OxMaterialAssembly&>(m_geometry_blueprint->material_assembly).isValid())
    {
        OxMaterialAssemblyAttorney<OxGeometry>::updateMaterialAssembly(m_geometry_blueprint->material_assembly, top_scene_object);
        getOxProgramFromDeclarationOffset().setVariableValue("num_materials",
            static_cast<unsigned int>(static_cast<OxMaterialAssembly&>(m_geometry_blueprint->material_assembly).getMaterialCount()));
    }
}

unsigned int OxGeometry::getPrimitiveCount() const
{
    unsigned int num_primitives{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGetPrimitiveCount(m_geometry_blueprint->native_handle, &num_primitives));
    return num_primitives;
}
