#ifndef RA_RENDERING_PASSES_SCATTERING_RENDERING_PASS
#define RA_RENDERING_PASSES_SCATTERING_RENDERING_PASS

#include "rendering_pass.h"
#include "../material_assembly.h"
#include "../miss_shader_assembly.h"
#include "../traverse_backup_buffer.h"
#include "../ray_casters/recaster_generator.h"


namespace ra { namespace rendering_passes {

class RaScatteringRenderingPass : public RaRenderingPass,  public RaContractWithRaPrograms
{
public:
    RaScatteringRenderingPass(
        RaSceneSection& target_scene_section, 
        RaRayGenerator const& ray_caster,
        uint8_t num_spectra_pairs_supported, 
        uint32_t max_recursion_depth,
        float ray_marching_step_size, 
        uint32_t num_scattering_integral_importance_directions,
        RaProgram const& absorption_probability_shader, 
        RaProgram const& scattering_probability_shader,
        RaProgram const& scattering_phase_function_shader);

    RaScatteringRenderingPass(
        RaSceneSection& target_scene_section, 
        RaRayGenerator const& ray_caster,
        uint8_t num_spectra_pairs_supported, 
        uint32_t max_recursion_depth,
        float ray_marching_step_size, 
        uint32_t num_scattering_integral_importance_directions);

    float2* mapImportanceDirectionsBuffer() const;
    void unmapImportanceDirectionsBuffer() const;

    uint32_t getMaxRecursionDepth() const;
    void setMaxRecursionDepth(uint32_t max_recursion_depth);

    uint32_t getNumberOfScatteringIntegralImportanceDirections() const;
    uint32_t getNumberOfSpectraPairsSupported() const;

    float getRayMarchingStepSize() const;
    void setRayMarchingStepSize(float step_size);

    RaProgram getAbsorptionProbabilityShader() const;
    void setAbsorptionProbabilityShader(RaProgram const& absorption_probability_shader);

    RaProgram getScatteringProbabilityShader() const;
    void setScatteringProbabilityShader(RaProgram const& scattering_probability_shader);

    RaProgram getScatteringPhaseFunctionShader() const;
    void setScatteringPhaseFunctionShader(RaProgram const& scattering_phase_function_shader);

protected:
    // Required by RaRenderingPass interface
    void prepare_impl() override;
    void render_impl() const override;

private:
    RaRayGenerator const& m_ray_caster;
    uint32_t m_num_spectra_pairs_supported;
    uint32_t m_max_recursion_depth;
    float m_ray_marching_step_size;
    uint32_t m_num_scattering_integral_importance_directions;
    RaMaterialAssembly m_surface_material_assembly;
    RaMissShaderAssembly m_miss_shader_assembly;
    RaTraverseBackupBuffer m_traverse_backup_buffer;
    RaBuffer<float2> m_importance_directions_buffer;
    ray_casters::RaRecasterGenerator m_recaster;
};

}}

#endif
