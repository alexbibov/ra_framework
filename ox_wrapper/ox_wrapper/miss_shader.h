#ifndef OX_WRAPPER_MISS_SHADER_H
#define OX_WRAPPER_MISS_SHADER_H

#include "fwd.h"
#include "contract_with_context.h"
#include "contract_with_programs.h"
#include "entity.h"

#include "ray_payloads.h"

namespace ox_wrapper {

template<typename T>
class OxMissShaderAttorney;

class OxMissShader : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxMissShaderAttorney<OxMissShaderAssembly>;

public:
    OxMissShader(OxProgram const& miss_shader, 
        OxRayTypeCollection const& supported_ray_types = OxRayTypeCollection{ 1U, OxRayType::unknown });
    virtual ~OxMissShader() = default;

    OxProgram getProgram() const;
    OxRayTypeCollection supportedRayTypes() const;
    bool supportsRayType(OxRayType ray_type) const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void apply(OxObjectHandle top_scene_object) const;

private:
    OxRayTypeCollection m_supported_ray_types;
};

template<>
class OxMissShaderAttorney<OxMissShaderAssembly>
{
    friend class OxMissShaderAssembly;

    static void applyMissShader(OxMissShader const& parent_miss_shader, OxObjectHandle top_scene_object)
    {
        parent_miss_shader.apply(top_scene_object);
    }


};

}

#endif
