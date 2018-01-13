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
    OxMissShader(OxProgram const& miss_shader, OxRayType ray_type = OxRayType::unknown);
    virtual ~OxMissShader() = default;

    OxProgram getProgram() const;
    OxRayType rayType() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void apply() const;

private:
    OxRayType m_ray_type;
};

template<>
class OxMissShaderAttorney<OxMissShaderAssembly>
{
    friend class OxMissShaderAssembly;

    static void applyMissShader(OxMissShader const& parent_miss_shader)
    {
        parent_miss_shader.apply();
    }
};

}

#endif
