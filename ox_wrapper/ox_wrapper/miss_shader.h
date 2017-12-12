#ifndef OX_WRAPPER_MISS_SHADER_H
#define OX_WRAPPER_MISS_SHADER_H

#include "fwd.h"
#include "contract_with_context.h"
#include "contract_with_programs.h"
#include "entity.h"

namespace ox_wrapper {

template<typename T>
class OxMissShaderAttorney;

class OxMissShader : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxMissShaderAttorney<OxMissShaderAssembly>;

public:
    OxMissShader(OxProgram const& miss_shader, unsigned int ray_type);

    OxProgram getMissShader() const;
    unsigned int rayType() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void apply() const;

private:
    unsigned int const m_ray_type;
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
