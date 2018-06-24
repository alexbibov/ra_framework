#ifndef RA_MISS_SHADER_H
#define RA_MISS_SHADER_H

#include "ra/ra_fwd.h"
#include "ra/contract_with_context.h"
#include "ra/contract_with_programs.h"
#include "ra/entity.h"

#include "ra/ray_payloads.h"

namespace ra {

template<typename T>
class RaMissShaderAttorney;

class RaMissShader : public RaContractWithRaContext, public RaContractWithRaPrograms, public RaEntity
{
    friend class RaMissShaderAttorney<RaMissShaderAssembly>;

public:
    RaMissShader(RaProgram const& miss_shader, 
        RaRayTypeCollection const& supported_ray_types = RaRayTypeCollection{ 1U, RaRayType::unknown });
    virtual ~RaMissShader() = default;

    RaProgram getProgram() const;
    RaRayTypeCollection supportedRayTypes() const;
    bool supportsRayType(RaRayType ray_type) const;

    // required by RaEntity interface
    bool isValid() const override;

private:
    void apply(RaObjectHandle top_scene_object) const;

private:
    RaRayTypeCollection m_supported_ray_types;
};

template<>
class RaMissShaderAttorney<RaMissShaderAssembly>
{
    friend class RaMissShaderAssembly;

    static void applyMissShader(RaMissShader const& parent_miss_shader, RaObjectHandle top_scene_object)
    {
        parent_miss_shader.apply(top_scene_object);
    }


};

}

#endif
