#ifndef OX_WRAPPER_RAY_GENERATOR_H
#define OX_WRAPPER_RAY_GENERATOR_H

#include <cstdint>
#include "fwd.h"
#include "contract_with_context.h"
#include "contract_with_programs.h"
#include "entity.h"


namespace ox_wrapper {

template<typename T>
class OxRayGeneratorAttorney;

class OxRayGenerator : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxRayGeneratorAttorney<OxSceneSection>;

public:
    OxRayGenerator(OxProgram const& optix_ray_generation_shader, uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);

    OxRayGenerator(OxProgram const& optix_ray_generation_shader, OxProgram const& optix_miss_shader,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);

    OxProgram getRayGenerationShader() const;
    OxProgram getMissShader() const;

    // required by OxEntity interface
    bool isValid() const override;

protected:
    void setGeneratorDimensions(uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z);

private:
    void update() const;
    void launch() const;

private:
    uint32_t m_num_rays_x;
    uint32_t m_num_rays_y;
    uint32_t m_num_rays_z;
    uint32_t m_entry_point_index;
};

template<>
class OxRayGeneratorAttorney<OxSceneSection>
{
    friend class OxSceneSection;

    static void updateRayGenerator(OxRayGenerator const& parent_ray_generator)
    {
        parent_ray_generator.update();
    }

    static void launchRayGenerator(OxRayGenerator const& parent_ray_generator)
    {
        parent_ray_generator.launch();
    }
};

};

#endif
