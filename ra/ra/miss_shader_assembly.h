#ifndef RA_MISS_SHADER_ASSEMBLY_H
#define RA_MISS_SHADER_ASSEMBLY_H

#include "ra/ra_fwd.h"
#include "ra/miss_shader.h"
#include "ra/util/optional.h"

#include <unordered_set>
#include <vector>

namespace ra {

template<typename T>
class RaMissShaderAssemblyAttorney;

class RaMissShaderAssembly : public RaContractWithRaContext, public RaEntity
{
    friend class RaMissShaderAssemblyAttorney<RaRayGenerator>;

public:
    RaMissShaderAssembly(std::initializer_list<RaMissShader> init_list);
    RaMissShaderAssembly(std::vector<RaMissShader> const& miss_shaders);
    virtual ~RaMissShaderAssembly() = default;

    util::Optional<RaMissShader> getMissShaderById(RaEntityId const& id) const;
    util::Optional<RaMissShader> getMissShaderByName(std::string const& name) const;
    util::Optional<RaMissShader> getMissShaderByRayType(RaRayType ray_type) const;

    // required by RaEntity interface
    bool isValid() const override;

private:
    struct miss_shader_hasher
    {
        uint64_t operator()(RaMissShader const& ms) const;
        bool operator()(RaMissShader const& ms1, RaMissShader const& ms2) const;
    };

    using miss_shader_collection = std::unordered_set<RaMissShader, 
        miss_shader_hasher, miss_shader_hasher>;

private:
    void apply(RaObjectHandle top_scene_object) const;

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
class RaMissShaderAssemblyAttorney<RaRayGenerator>
{
    friend class RaRayGenerator;

    static void applyMissShaderAssembly(RaMissShaderAssembly const& parent_miss_shader_assembly, 
        RaObjectHandle top_scene_object)
    {
        parent_miss_shader_assembly.apply(top_scene_object);
    }
};

}

#endif
