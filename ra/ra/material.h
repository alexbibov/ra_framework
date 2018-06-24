#ifndef OX_WRAPPER_MATERIAL_H
#define OX_WRAPPER_MATERIAL_H

#include "optix.h"

#include "ra/ra_fwd.h"
#include "ra/contract_with_context.h"
#include "ra/contract_with_programs.h"
#include "ra/entity.h"
#include "ra/ray_payloads.h"
#include "ra/util/optional.h"

#include <string>
#include <memory>

namespace ra {

template<typename T>
class OxMaterialAttorney;

class OxMaterial : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxMaterialAttorney<OxMaterialAssembly>;

public:
    OxMaterial(
        util::Optional<OxProgram> const& closest_hit_shader,
        util::Optional<OxProgram> const& any_hit_shader,
        OxRayTypeCollection const& supported_ray_types = OxRayTypeCollection{ 1U, OxRayType::unknown });
    virtual ~OxMaterial() = default;

    util::Optional<OxProgram> getClosestHitShader() const;
    util::Optional<OxProgram> getAnyHitShader() const;
    OxRayTypeCollection supportedRayTypes() const;
    bool supportsRayType(OxRayType ray_type) const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void update(OxObjectHandle top_scene_object) const;

private:
    std::shared_ptr<RTmaterial_api> m_native_material;
    OxRayTypeCollection m_supported_ray_types;

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
