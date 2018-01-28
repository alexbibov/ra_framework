#include "context.h"
#include "material_assembly.h"
#include "contract_with_context.h"

using namespace ox_wrapper;


OxMaterialAssembly::OxMaterialAssembly():
    OxContractWithOxContext{*reinterpret_cast<OxContext const*>(0)},
    m_is_dummy{ true }
{
}

OxMaterialAssembly::OxMaterialAssembly(std::initializer_list<OxMaterial> init_list):
    OxMaterialAssembly{ std::vector<OxMaterial>{init_list} }
{
    
}

OxMaterialAssembly::OxMaterialAssembly(std::vector<OxMaterial> const& materials):
    OxContractWithOxContext{ materials.begin()->context() },
    m_is_dummy{ false }
{
    RTgeometryinstance native_geometry_instance_handle;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryInstanceCreate(nativeOptiXContextHandle(), &native_geometry_instance_handle));
    m_native_geometry_instance.reset(native_geometry_instance_handle,
        [this](RTgeometryinstance gi)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryInstanceDestroy(gi));
    });

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
        rtGeometryInstanceSetMaterialCount(native_geometry_instance_handle, static_cast<unsigned int>(materials.size())));

    unsigned int idx{ 0U };
    for (auto const& e : materials)
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
            rtGeometryInstanceSetMaterial(native_geometry_instance_handle, idx, OxMaterialAttorney<OxMaterialAssembly>::getNativeMaterialHandle(e)));

        if (!m_materials.insert(e).second)
            util::Log::retrieve()->out("Material \"" + e.getStringName() + "\" cannot be added into material assembly (material for ray type \""
                + std::to_string(static_cast<unsigned int>(e.rayType())) + "\" already exists in this assembly)", util::LogMessageType::exclamation);

        ++idx;
    }
}

util::Optional<OxMaterial> OxMaterialAssembly::getMaterialById(OxEntityId const& id) const
{
    for (auto const& e : m_materials)
    {
        if (e.getUniqueIdentifier() == id)
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

size_t OxMaterialAssembly::getMaterialCount() const
{
    if(!m_is_dummy)
    {
        unsigned int rv{};
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryInstanceGetMaterialCount(m_native_geometry_instance.get(), &rv));
        return rv;
    }
    return 0U;
}

bool OxMaterialAssembly::isValid() const
{
    if (m_is_dummy) return true;

    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtGeometryInstanceValidate(m_native_geometry_instance.get()));
    
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
