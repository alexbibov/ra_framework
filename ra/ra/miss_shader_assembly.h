#ifndef OX_WRAPPER_MISS_SHADER_ASSEMBLY_H
#define OX_WRAPPER_MISS_SHADER_ASSEMBLY_H

#include "ra/ra_fwd.h"
#include "ra/miss_shader.h"
#include "ra/util/optional.h"

#include <unordered_set>
#include <vector>

namespace ra {

template<typename T>
class OxMissShaderAssemblyAttorney;

class OxMissShaderAssembly : public OxContractWithOxContext, public OxEntity
{
    friend class OxMissShaderAssemblyAttorney<OxRayGenerator>;

public:
    OxMissShaderAssembly(std::initializer_list<OxMissShader> init_list);
    OxMissShaderAssembly(std::vector<OxMissShader> const& miss_shaders);
    virtual ~OxMissShaderAssembly() = default;

    util::Optional<OxMissShader> getMissShaderById(OxEntityId const& id) const;
    util::Optional<OxMissShader> getMissShaderByName(std::string const& name) const;
    util::Optional<OxMissShader> getMissShaderByRayType(OxRayType ray_type) const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    struct miss_shader_hasher
    {
        uint64_t operator()(OxMissShader const& ms) const;
        bool operator()(OxMissShader const& ms1, OxMissShader const& ms2) const;
    };

    using miss_shader_collection = std::unordered_set<OxMissShader, 
        miss_shader_hasher, miss_shader_hasher>;

private:
    void apply(OxObjectHandle top_scene_object) const;

private:
    miss_shader_collection m_miss_shader_list;

public:
    miss_shader_collection::iterator begin();
    miss_shader_collection::iterator end();

    miss_shader_collection::const_iterator cbegin();
    miss_shader_collection::const_iterator cend();

    miss_shader_collection::const_iterator begin() const;
    miss_shader_collection::const_iterator end() const;
};

template<>
class OxMissShaderAssemblyAttorney<OxRayGenerator>
{
    friend class OxRayGenerator;

    static void applyMissShaderAssembly(OxMissShaderAssembly const& parent_miss_shader_assembly, 
        OxObjectHandle top_scene_object)
    {
        parent_miss_shader_assembly.apply(top_scene_object);
    }
};

}

#endif
