#ifndef OX_WRAPPER_MISS_SHADER_ASSEMBLY_H
#define OX_WRAPPER_MISS_SHADER_ASSEMBLY_H

#include "fwd.h"
#include "miss_shader.h"

#include <vector>

namespace ox_wrapper {

template<typename T>
class OxMissShaderAssemblyAttorney;

class OxMissShaderAssembly : public OxContractWithOxContext, public OxEntity
{
    friend class OxMissShaderAssemblyAttorney<OxRayGenerator>;

public:
    OxMissShaderAssembly(std::initializer_list<OxMissShader> init_list);

    OxMissShader const* getMissShaderById(OxEntityID const& id) const;
    OxMissShader const* getMissShaderByName(std::string const& name) const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    void apply() const;

private:
    std::vector<OxMissShader> m_miss_shader_list;
};

template<>
class OxMissShaderAssemblyAttorney<OxRayGenerator>
{
    friend class OxRayGenerator;

    static void applyMissShaderAssembly(OxMissShaderAssembly const& parent_miss_shader_assembly)
    {
        parent_miss_shader_assembly.apply();
    }
};

}

#endif
