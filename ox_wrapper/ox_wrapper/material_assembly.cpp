#include "context.h"
#include "material_assembly.h"
#include "contract_with_context.h"
#include "util/misc.h"

#include <algorithm>


using namespace ox_wrapper;


OxMaterialAssembly::OxMaterialAssembly(OxContext const& context):
    OxContractWithOxContext{ context },
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

    uint64_t used_material_slots_mask = 0U;
    for(unsigned int idx = 0U; idx < materials.size(); ++idx)
    {
        OxMaterial const& current = materials[idx];
        
        uint64_t current_mask = rayTypeCollectionTo64BitMask(current.supportedRayTypes());
        uint64_t repeated_uses_mask = current_mask & used_material_slots_mask;
        if (repeated_uses_mask)
        {
            auto repeated_uses_indices = util::misc::getSetBits(repeated_uses_mask);
            std::string log_message = "Material \"" + current.getStringName() + "\" could not be added into material "
                "assembly \"" + getStringName() + "\". Materials for ray types ";

            std::for_each(repeated_uses_indices.begin(), --repeated_uses_indices.end(),
                [&log_message](uint8_t e) { log_message += e + ", "; });
            log_message += repeated_uses_indices[repeated_uses_indices.size() - 1];

            log_message += " have already been assigned in this material assembly";

            throw OxException{ log_message.c_str(), __FILE__, __FUNCTION__, __LINE__ };
        }

        THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
            rtGeometryInstanceSetMaterial(native_geometry_instance_handle, idx,
                OxMaterialAttorney<OxMaterialAssembly>::getNativeMaterialHandle(current)));

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
        uint64_t supported_ray_types_mask = rayTypeCollectionTo64BitMask(e.supportedRayTypes());
        if (supported_ray_types_mask & static_cast<uint64_t>(ray_type))
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

uint64_t OxMaterialAssembly::material_hasher::operator()(OxMaterial const& m) const
{
    return rayTypeCollectionTo64BitMask(m.supportedRayTypes());
}

bool OxMaterialAssembly::material_hasher::operator()(OxMaterial const& m1, OxMaterial const& m2) const
{
    return 
        static_cast<OxProgram&>(m1.getClosestHitShader()).getId().native ==
        static_cast<OxProgram&>(m2.getClosestHitShader()).getId().native
        && static_cast<OxProgram&>(m1.getAnyHitShader()).getId().native ==
        static_cast<OxProgram&>(m2.getAnyHitShader()).getId().native;
}
