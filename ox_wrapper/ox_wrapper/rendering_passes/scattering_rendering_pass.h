#ifndef OX_WRAPPER_RENDERING_PASSES_SCATTERING_RENDERING_PASS
#define OX_WRAPPER_RENDERING_PASSES_SCATTERING_RENDERING_PASS

#include "rendering_pass.h"
#include "../material_assembly.h"
#include "../miss_shader_assembly.h"
#include "../traverse_backup_buffer.h"
#include "../ray_casters/recaster_generator.h"


namespace ox_wrapper { namespace rendering_passes {

class OxScatteringRenderingPass : public OxRenderingPass,  public OxContractWithOxPrograms
{
public:
    OxScatteringRenderingPass(
        OxSceneSection const& target_scene_section, 
        OxRayGenerator const& ray_caster,
        uint8_t num_spectra_pairs_supported, 
        uint32_t max_recursion_depth,
        float ray_marching_step_size, 
        uint32_t num_scattering_integral_importance_directions,
        OxProgram const& absorption_probability_shader, 
        OxProgram const& scattering_probability_shader,
        OxProgram const& scattering_phase_function_shader);

    OxScatteringRenderingPass(
        OxSceneSection const& target_scene_section, 
        OxRayGenerator const& ray_caster,
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

    OxProgram getAbsorptionProbabilityShader() const;
    void setAbsorptionProbabilityShader(OxProgram const& absorption_probability_shader);

    OxProgram getScatteringProbabilityShader() const;
    void setScatteringProbabilityShader(OxProgram const& scattering_probability_shader);

    OxProgram getScatteringPhaseFunctionShader() const;
    void setScatteringPhaseFunctionShader(OxProgram const& scattering_phase_function_shader);

    void render() const override;

private:
    OxRayGenerator const& m_ray_caster;
    uint32_t m_num_spectra_pairs_supported;
    uint32_t m_max_recursion_depth;
    float m_ray_marching_step_size;
    uint32_t m_num_scattering_integral_importance_directions;
    OxMaterialAssembly m_surface_material_assembly;
    OxMissShaderAssembly m_miss_shader_assembly;
    OxTraverseBackupBuffer m_traverse_backup_buffer;
    ray_casters::OxRecasterGenerator m_recaster_ray_generator;
    OxBuffer<float2> m_importance_directions_buffer;
};

}}

#endif
