#ifndef OX_WRAPPER_MATERIAL_H
#define OX_WRAPPER_MATERIAL_H

#include "fwd.h"
#include "optix.h"
#include "contract_with_context.h"
#include "contract_with_programs.h"
#include "entity.h"

#include <string>
#include <memory>

namespace ox_wrapper {

template<typename T>
class OxMaterialAttorney;

class OxMaterial : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxMaterialAttorney<OxMaterialAssembly>;

public:
    OxMaterial(OxProgram const& closest_hit_shader, unsigned int ray_type_index = 0U);
    OxMaterial(OxProgram const& closest_hit_shader, OxProgram const& any_hit_shader, unsigned int ray_type_index = 0U);

    OxProgram getClosestHitShader() const;
    OxProgram getAnyHitShader() const;
    unsigned int rayType() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    std::shared_ptr<RTmaterial_api> m_native_material;
    unsigned int const m_ray_type_index;
};

template<>
class OxMaterialAttorney<OxMaterialAssembly>
{
    friend class OxMaterialAssembly;

    static RTmaterial getNativeMaterialHandle(OxMaterial const& parent_material)
    {
        return parent_material.m_native_material.get();
    }
};

}

#endif
