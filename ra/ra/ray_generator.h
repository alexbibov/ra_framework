#ifndef OX_WRAPPER_RAY_GENERATOR_H
#define OX_WRAPPER_RAY_GENERATOR_H

#include <cstdint>
#include "ra/ra_fwd.h"
#include "ra/contract_with_context.h"
#include "ra/contract_with_programs.h"
#include "ra/entity.h"
#include "ra/miss_shader_assembly.h"
#include "ra/util/optional.h"


namespace ra {

template<typename T>
class OxRayGeneratorAttorney;

class OxRayGenerator : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxRayGeneratorAttorney<OxSceneSection>;

public:
    OxRayGenerator(OxProgram const& ray_generation_shader, 
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U, 
        uint32_t entry_point_index = 0U);

    OxRayGenerator(OxProgram const& ray_generation_shader, OxMissShaderAssembly const& miss_shader_assembly,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);


    OxProgram getRayGenerationShader() const;
    util::Optional<OxMissShaderAssembly> getMissShaderAssembly() const;

    void setMissShaderAssembly(OxMissShaderAssembly const& miss_shader_assembly) const;

    // required by OxEntity interface
    bool isValid() const override;

    uint3 getGeneratorDimensions() const;
    unsigned int numberOfRays() const;

    virtual OxAbstractBuffer const& outputBuffer() const = 0;

protected:
    void setGeneratorDimensions(uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z);

    virtual void update(OxObjectHandle top_scene_object) const;
    virtual void launch() const;

    void update_topology(uint32_t new_num_rays_x, 
        uint32_t new_num_rays_y, 
        uint32_t new_num_rays_z, uint32_t new_entry_point_index = 0U) const;

private:
    std::shared_ptr<util::Optional<OxMissShaderAssembly>> m_p_miss_shader_assembly;

    mutable uint32_t m_num_rays_x;
    mutable uint32_t m_num_rays_y;
    mutable uint32_t m_num_rays_z;
    mutable uint32_t m_entry_point_index;
};

template<>
class OxRayGeneratorAttorney<OxSceneSection>
{
    friend class OxSceneSection;

    static void updateRayGenerator(OxRayGenerator const& parent_ray_generator, OxObjectHandle top_scene_object)
    {
        parent_ray_generator.update(top_scene_object);
    }

    static void launchRayGenerator(OxRayGenerator const& parent_ray_generator)
    {
        parent_ray_generator.launch();
    }
};

};

#endif
