#include "context.h"
#include "geometry.h"

using namespace ra;

RaGeometry::RaGeometry(RaProgram const& intersection_shader, RaProgram const& aabb_shader):
    RaContractWithRaContext{ intersection_shader.context() },
    RaContractWithRaPrograms{ intersection_shader, aabb_shader }
{
    RTgeometry native_geometry_handle{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryCreate(nativeOptiXContextHandle(), &native_geometry_handle));
    m_geometry_blueprint.reset(new geometry_blueprint{ native_geometry_handle, true, util::Optional<RaMaterialAssembly>{} },
        [this](geometry_blueprint* e)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryDestroy(e->native_handle));
        delete e;
    });

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometrySetIntersectionProgram(native_geometry_handle, nativeOptiXProgramHandle(0U)));
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometrySetBoundingBoxProgram(native_geometry_handle, nativeOptiXProgramHandle(1U)));
}

RaGeometry::RaGeometry(RaProgram const& intersection_shader, RaProgram const& aabb_shader, RaMaterialAssembly const& material_assembly):
    RaGeometry{ intersection_shader, aabb_shader }
{
    setMaterialAssembly(material_assembly);
}

util::Optional<RaMaterialAssembly> RaGeometry::getMaterialAssembly() const
{
    return m_geometry_blueprint->material_assembly;
}

void RaGeometry::setMaterialAssembly(RaMaterialAssembly const& material_assembly) const
{
    m_geometry_blueprint->material_assembly = material_assembly;
    RaMaterialAssemblyAttorney<RaGeometry>::attachMaterialAssemblyToNativeGeometryHandle(material_assembly, m_geometry_blueprint->native_handle);
    m_geometry_blueprint->is_dirty = true;
}

RaProgram RaGeometry::getAABBShader() const
{
    return getRaProgramFromDeclarationOffset(0U);
}

RaProgram RaGeometry::getIntersectionShader() const
{
    return getRaProgramFromDeclarationOffset(1U);
}

bool RaGeometry::isValid() const
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
        && static_cast<RaMaterialAssembly&>(m_geometry_blueprint->material_assembly).isValid();
}

void RaGeometry::setPrimitiveCount(unsigned int num_primitives)
{
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometrySetPrimitiveCount(m_geometry_blueprint->native_handle, num_primitives));
    markDirty();
}

void RaGeometry::markDirty()
{
    m_geometry_blueprint->is_dirty = true;
}

void RaGeometry::update(RaObjectHandle top_scene_object) const
{
    if (m_geometry_blueprint->material_assembly.isValid()
        && static_cast<RaMaterialAssembly&>(m_geometry_blueprint->material_assembly).isValid())
    {
        RaMaterialAssemblyAttorney<RaGeometry>::updateMaterialAssembly(m_geometry_blueprint->material_assembly, top_scene_object);
        getRaProgramFromDeclarationOffset().setVariableValue("ra_num_materials",
            static_cast<unsigned int>(static_cast<RaMaterialAssembly&>(m_geometry_blueprint->material_assembly).getMaterialCount()));
    }
}

unsigned int RaGeometry::getPrimitiveCount() const
{
    unsigned int num_primitives{};
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGetPrimitiveCount(m_geometry_blueprint->native_handle, &num_primitives));
    return num_primitives;
}
