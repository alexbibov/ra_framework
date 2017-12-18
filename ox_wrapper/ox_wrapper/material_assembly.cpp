#include "context.h"
#include "material_assembly.h"
#include "contract_with_context.h"

using namespace ox_wrapper;


OxMaterialAssembly::OxMaterialAssembly(std::initializer_list<OxMaterial> init_list):
    OxContractWithOxContext{ init_list.begin()->context() }
{
    RTgeometryinstance native_geometry_instance_handle;
    throwOptiXContextError(rtGeometryInstanceCreate(nativeOptiXContextHandle(), &native_geometry_instance_handle));
    m_native_geometry_instance.reset(native_geometry_instance_handle,
        [this](RTgeometryinstance gi)->void
    {
        logOptiXContextError(rtGeometryInstanceDestroy(gi));
    });

    throwOptiXContextError(rtGeometryInstanceSetMaterialCount(native_geometry_instance_handle, static_cast<unsigned int>(init_list.size())));
    
    unsigned int idx{ 0U };
    for (auto const& e : init_list)
    {
        throwOptiXContextError(rtGeometryInstanceSetMaterial(native_geometry_instance_handle, idx, OxMaterialAttorney<OxMaterialAssembly>::getNativeMaterialHandle(e)));

        if (!m_materials.insert(e).second)
            util::Log::retrieve()->out("Material \"" + e.getStringName() + "\" cannot be added into material assembly (material for ray type \""
                + std::to_string(static_cast<unsigned int>(e.rayType())) + "\" already exists in this assembly)", util::LogMessageType::exclamation);

        ++idx;
    }
}

util::Optional<OxMaterial> OxMaterialAssembly::getMaterialById(OxEntityID const& id) const
{
    for (auto const& e : m_materials)
    {
        if (e.getId() == id)
            return e;
    }

    return util::Optional<OxMaterial>{};
}

util::Optional<OxMaterial> OxMaterialAssembly::getMaterialByName(std::string const& name) const
{
    for (auto const& e : m_materials)
    {
        if (e.getStringName() == name)
            return e;
    }

    return util::Optional<OxMaterial>{};
}

util::Optional<OxMaterial> OxMaterialAssembly::getMaterialByRayType(OxRayType ray_type) const
{
    for (auto const& e : m_materials)
    {
        if (e.rayType() == ray_type)
            return e;
    }

    return util::Optional<OxMaterial>();
}

bool OxMaterialAssembly::isValid() const
{
    RTresult res = rtGeometryInstanceValidate(m_native_geometry_instance.get());
    logOptiXContextError(res);
    
    bool materials_valid{ true };
    for (auto const& e : m_materials)
    {
        if (!e.isValid())
        {
            materials_valid = false;
            break;
        }
    }

    return res == RT_SUCCESS && materials_valid;
}

void OxMaterialAssembly::update(OxObjectHandle top_scene_object) const
{
    for (auto& m : m_materials)
    {
        OxMaterialAttorney<OxMaterialAssembly>::updateMaterial(m, top_scene_object);
    }
}

OxMaterialAssembly::material_collection::iterator OxMaterialAssembly::begin()
{
    return m_materials.begin();
}

OxMaterialAssembly::material_collection::iterator OxMaterialAssembly::end()
{
    return m_materials.end();
}

OxMaterialAssembly::material_collection::const_iterator OxMaterialAssembly::cbegin()
{
    return m_materials.cbegin();
}

OxMaterialAssembly::material_collection::const_iterator OxMaterialAssembly::cend()
{
    return m_materials.cend();
}

OxMaterialAssembly::material_collection::const_iterator OxMaterialAssembly::begin() const
{
    return m_materials.begin();
}

OxMaterialAssembly::material_collection::const_iterator OxMaterialAssembly::end() const
{
    return m_materials.end();
}

bool OxMaterialAssembly::material_comparator::operator()(OxMaterial const& m1, OxMaterial const& m2) const
{
    return m1.rayType() < m2.rayType();
}
