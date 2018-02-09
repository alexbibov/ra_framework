#ifndef OX_WRAPPER_RENDERING_PASSES_RENDERING_PASSES_FACTORY_H
#define OX_WRAPPER_RENDERING_PASSES_RENDERING_PASSES_FACTORY_H

#include "../entity.h"
#include "fwd.h"
#include "scattering_rendering_pass.h"

namespace ox_wrapper { namespace rendering_passes {

class OxRenderingPassesFactory : public OxEntity
{
public:
    static OxRenderingPassesFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxRenderingPassesFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

    // OxScatteringRenderingPass
public:
    OxScatteringRenderingPass createScatteringRenderingPass(OxSceneSection const& target_scene_section, uint8_t num_spectra_pairs_supported, uint32_t max_recursion_depth,
        float ray_marching_step_size, uint32_t num_scattering_integral_importance_directions,
        OxProgram const& absorption_probability_shader, OxProgram const& scattering_probability_shader,
        OxProgram const& scattering_phase_function_shader);

    OxScatteringRenderingPass createScatteringRenderingPass(OxSceneSection const& target_scene_section, uint8_t num_spectra_pairs_supported, uint32_t max_recursion_depth,
        float ray_marching_step_size, uint32_t num_scattering_integral_importance_directions);

private:
    OxRenderingPassesFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}}

#endif