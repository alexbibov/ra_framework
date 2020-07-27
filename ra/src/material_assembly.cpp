#include "context.h"
#include "material_assembly.h"
#include "contract_with_context.h"
#include "util/misc.h"

#include <algorithm>


using namespace ra;


RaMaterialAssembly::RaMaterialAssembly(RaContext const& context):
    RaContractWithRaContext{ context },
    m_is_dummy{ true }
{
}

RaMaterialAssembly::RaMaterialAssembly(std::initializer_list<RaMaterial> init_list):
    RaMaterialAssembly{ std::vector<RaMaterial>{init_list} }
{
    
}

RaMaterialAssembly::RaMaterialAssembly(std::vector<RaMaterial> const& materials):
    RaContractWithRaContext{ materials.begin()->context() },
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
        RaMaterial const& current = materials[idx]; 
        
        uint64_t current_mask = rayTypeCollectionTo64BitMask(current.supportedRayTypes());
        uint64_t repeated_uses_mask = current_mask & used_material_slots_mask;
        if (repeated_uses_mask)
        {
            auto repeated_uses_indices = util::misc::getSetBits(repeated_uses_mask);
            std::string log_message = "Material \"" + current.getStringName() + "\" could not be added into material "
                "assembly \"" + getStringName() + "\". Materials for ray types ";

            std::for_each(repeated_uses_indices.begin(), --repeated_uses_indices.end(),
                [&log_message](uint8_t e) { log_message += std::to_string(e) + ", "; });
            log_message += std::to_string(repeated_uses_indices[repeated_uses_indices.size() - 1]);

            log_message += " have already been assigned in this material assembly";

            THROW_RA_ERROR(log_message);
        }

        THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
            rtGeometryInstanceSetMaterial(native_geometry_instance_handle, idx,
                RaMaterialAttorney<RaMaterialAssembly>::getNativeMaterialHandle(current)));
        m_materials.insert(current);
        
        used_material_slots_mask |= current_mask;
    } 
}

util::Optional<RaMaterial> RaMaterialAssembly::getMaterialById(RaEntityId const& id) const
{
    for (auto const& e : m_materials)
    {
        if (e.getUniqueIdentifier() == id)
            return e;
    }

    return util::Optional<RaMaterial>{};
}

util::Optional<RaMaterial> RaMaterialAssembly::getMaterialByName(std::string const& name) const
{
    for (auto const& e : m_materials)
    {
        if (e.getStringName() == name)
            return e;
    }

    return util::Optional<RaMaterial>{};
}

util::Optional<RaMaterial> RaMaterialAssembly::getMaterialByRayType(RaRayType ray_type) const
{
    for (auto const& e : m_materials)
    {
        uint64_t supported_ray_types_mask = rayTypeCollectionTo64BitMask(e.supportedRayTypes());
        if (supported_ray_types_mask & (0x1ui64 << static_cast<unsigned>(ray_type)))
            return e;
    }

    return util::Optional<RaMaterial>();
}

size_t RaMaterialAssembly::getMaterialCount() const
{
    if(!m_is_dummy)
    {
        unsigned int rv{};
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryInstanceGetMaterialCount(m_native_geometry_instance.get(), &rv));
        return rv;
    }
    return 0U;
}

bool RaMaterialAssembly::isValid() const
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

void RaMaterialAssembly::update(RaObjectHandle top_scene_object) const
{
    for (auto& m : m_materials)
    {
        RaMaterialAttorney<RaMaterialAssembly>::updateMaterial(m, top_scene_object);
    }
}

RaMaterialAssembly::material_collection::iterator RaMaterialAssembly::begin()
{
    return m_materials.begin();
}

RaMaterialAssembly::material_collection::iterator RaMaterialAssembly::end()
{
    return m_materials.end();
}

RaMaterialAssembly::material_collection::const_iterator RaMaterialAssembly::cbegin()
{
    return m_materials.cbegin();
}

RaMaterialAssembly::material_collection::const_iterator RaMaterialAssembly::cend()
{
    return m_materials.cend();
}

RaMaterialAssembly::material_collection::const_iterator RaMaterialAssembly::begin() const
{
    return m_materials.begin();
}

RaMaterialAssembly::material_collection::const_iterator RaMaterialAssembly::end() const
{
    return m_materials.end();
}

uint64_t RaMaterialAssembly::material_hasher::operator()(RaMaterial const& m) const
{
    return rayTypeCollectionTo64BitMask(m.supportedRayTypes());
}

bool RaMaterialAssembly::material_hasher::operator()(RaMaterial const& m1, RaMaterial const& m2) const
{
    return 
        static_cast<RaProgram&>(m1.getClosestHitShader()).getId().native ==
        static_cast<RaProgram&>(m2.getClosestHitShader()).getId().native
        && static_cast<RaProgram&>(m1.getAnyHitShader()).getId().native ==
        static_cast<RaProgram&>(m2.getAnyHitShader()).getId().native;
}
