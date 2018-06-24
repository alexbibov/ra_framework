#ifndef RA_MATERIAL_H
#define RA_MATERIAL_H

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
class RaMaterialAttorney;

class RaMaterial : public RaContractWithRaContext, public RaContractWithRaPrograms, public RaEntity
{
    friend class RaMaterialAttorney<RaMaterialAssembly>;

public:
    RaMaterial(
        util::Optional<RaProgram> const& closest_hit_shader,
        util::Optional<RaProgram> const& any_hit_shader,
        RaRayTypeCollection const& supported_ray_types = RaRayTypeCollection{ 1U, RaRayType::unknown });
    virtual ~RaMaterial() = default;

    util::Optional<RaProgram> getClosestHitShader() const;
    util::Optional<RaProgram> getAnyHitShader() const;
    RaRayTypeCollection supportedRayTypes() const;
    bool supportsRayType(RaRayType ray_type) const;

    // required by RaEntity interface
    bool isValid() const override;

private:
    void update(RaObjectHandle top_scene_object) const;

private:
    std::shared_ptr<RTmaterial_api> m_native_material;
    RaRayTypeCollection m_supported_ray_types;

    int8_t m_closest_hit_program_offset;
    int8_t m_any_hit_program_offset;
};

template<>
class RaMaterialAttorney<RaMaterialAssembly>
{
    friend class RaMaterialAssembly;

    static RTmaterial getNativeMaterialHandle(RaMaterial const& parent_material)
    {
        return parent_material.m_native_material.get();
    }

    static void updateMaterial(RaMaterial const& parent_material, RaObjectHandle top_scene_object)
    {
        parent_material.update(top_scene_object);
    }
};

}

#endif
