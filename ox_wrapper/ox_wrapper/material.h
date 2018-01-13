#ifndef OX_WRAPPER_MATERIAL_H
#define OX_WRAPPER_MATERIAL_H

#include "fwd.h"
#include "optix.h"
#include "contract_with_context.h"
#include "contract_with_programs.h"
#include "entity.h"
#include "util/optional.h"

#include <string>
#include <memory>

#include "ray_payloads.h"

namespace ox_wrapper {

template<typename T>
class OxMaterialAttorney;

class OxMaterial : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxMaterialAttorney<OxMaterialAssembly>;

public:
    OxMaterial(util::Optional<OxProgram> const& closest_hit_shader, util::Optional<OxProgram> const& any_hit_shader, OxRayType ray_type = OxRayType::unknown);
    virtual ~OxMaterial() = default;

    util::Optional<OxProgram> getClosestHitShader() const;
    util::Optional<OxProgram> getAnyHitShader() const;
    OxRayType rayType() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void update(OxObjectHandle top_scene_object) const;

private:
    std::shared_ptr<RTmaterial_api> m_native_material;
    OxRayType m_ray_type;

    int8_t m_closest_hit_program_offset;
    int8_t m_any_hit_program_offset;
};

template<>
class OxMaterialAttorney<OxMaterialAssembly>
{
    friend class OxMaterialAssembly;

    static RTmaterial getNativeMaterialHandle(OxMaterial const& parent_material)
    {
        return parent_material.m_native_material.get();
    }

    static void updateMaterial(OxMaterial const& parent_material, OxObjectHandle top_scene_object)
    {
        parent_material.update(top_scene_object);
    }
};

}

#endif
