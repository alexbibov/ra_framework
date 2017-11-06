#ifndef OX_WRAPPER_OPTIX_RAY_GENERATOR_H
#define OX_WRAPPER_OPTIX_RAY_GENERATOR_H

#include <cstdint>
#include "ox_wrapper_fwd.h"
#include "has_contract_with_optix_context.h"
#include "has_contract_with_optix_program.h"


namespace ox_wrapper {

template<typename T>
class OptiXRayGeneratorAttorney;

class OptiXRayGenerator : public HasContractWithOptiXContext, public HasContractWithOptiXProgram
{
    friend class OptiXRayGeneratorAttorney<OptiXSceneSection>;

public:
    OptiXRayGenerator(OptiXProgram const& optix_ray_generation_program, uint32_t numRays_x, uint32_t numRays_y = 1U, uint32_t num_Rays_z = 1U,
        uint32_t entry_point_index = 0U);

private:
    void launch() const;

private:
    uint32_t m_num_rays_x;
    uint32_t m_num_rays_y;
    uint32_t m_num_rays_z;
    uint32_t m_entry_point_index;
};

template<>
class OptiXRayGeneratorAttorney<OptiXSceneSection>
{
    friend class OptiXSceneSection;

    static void launchRayGenerator(OptiXRayGenerator const& optix_ray_generator)
    {
        optix_ray_generator.launch();
    }
};

};

#endif
