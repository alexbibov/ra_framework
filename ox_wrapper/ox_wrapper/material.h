#ifndef OX_WRAPPER_MATERIAL_H
#define OX_WRAPPER_MATERIAL_H

#include "fwd.h"
#include "optix.h"
#include "contract_with_context.h"
#include "contract_with_programs.h"
#include "entity.h"

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
    OxMaterial(OxProgram const& closest_hit_shader, OxRayType ray_type = OxRayType::unknown);
    OxMaterial(OxProgram const& closest_hit_shader, OxProgram const& any_hit_shader, OxRayType ray_type = OxRayType::unknown);
    virtual ~OxMaterial() = default;

    OxProgram getClosestHitShader() const;
    OxProgram getAnyHitShader() const;
    OxRayType rayType() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void update(OxObjectHandle top_scene_object) const;

private:
    std::shared_ptr<RTmaterial_api> m_native_material;
    OxRayType m_ray_type;
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
