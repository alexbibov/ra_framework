#ifndef RA_RENDERING_PASSES_RENDERING_PASSES_FACTORY_H
#define RA_RENDERING_PASSES_RENDERING_PASSES_FACTORY_H

#include "entity.h"
#include "ra_fwd.h"
#include "rendering_passes/scattering_rendering_pass.h"

namespace ra { namespace rendering_passes {

class RaRenderingPassesFactory : public RaEntity
{
public:
    static RaRenderingPassesFactory* initialize(RaContext const& context);
    static void shutdown();
    static RaRenderingPassesFactory* retrieve();

public:
    // required by RaEntity interface
    bool isValid() const override;

    // RaScatteringRenderingPass
public:
    RaScatteringRenderingPass createScatteringRenderingPass(
        RaSceneSection& target_scene_section, 
        RaRayGenerator const& ray_caster,
        uint8_t num_spectra_pairs_supported,
        float ray_marching_step_size, 
        uint32_t num_scattering_integral_importance_directions,
        RaProgram const& absorption_probability_shader, 
        RaProgram const& scattering_probability_shader,
        RaProgram const& scattering_phase_function_shader);

    RaScatteringRenderingPass createScatteringRenderingPass(
        RaSceneSection& target_scene_section, 
        RaRayGenerator const& ray_caster,
        uint8_t num_spectra_pairs_supported,
        float ray_marching_step_size, 
        uint32_t num_scattering_integral_importance_directions);

private:
    RaRenderingPassesFactory(RaContext const& context);

private:
    RaContext const& m_context;
};

}}

#endif
