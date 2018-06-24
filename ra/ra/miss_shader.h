#ifndef OX_WRAPPER_MISS_SHADER_H
#define OX_WRAPPER_MISS_SHADER_H

#include "ra/ra_fwd.h"
#include "ra/contract_with_context.h"
#include "ra/contract_with_programs.h"
#include "ra/entity.h"

#include "ra/ray_payloads.h"

namespace ra {

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
