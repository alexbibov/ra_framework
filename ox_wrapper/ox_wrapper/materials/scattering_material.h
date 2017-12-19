#ifndef OX_WRAPPER_SCATTERING_MATERIAL
#define OX_WRAPPER_SCATTERING_MATERIAL

#include "../material_assembly.h"
#include "../miss_shader_assembly.h"
#include "../traverse_backup_buffer.h"

namespace ox_wrapper { namespace materials {

class OxScatteringMaterial : public OxContractWithOxContext, public OxContractWithOxPrograms
{
public:
    OxScatteringMaterial(uint32_t num_rays, uint8_t num_spectra_pairs_supported, uint32_t max_recursion_depth,
        float ray_marching_step_size, uint32_t num_scattering_integral_importance_directions,
        OxProgram const& absorption_probability_shader, OxProgram const& scattering_probability_shader,
        OxProgram const& scattering_phase_function_shader);

    OxScatteringMaterial(OxContext const& context, uint32_t num_rays, uint8_t num_spectra_pairs_supported, uint32_t max_recursion_depth,
        float ray_marching_step_size, uint32_t num_scattering_integral_importance_directions);

    float2* mapImportanceDirectionsBuffer() const;
    void unmapImportanceDirectionsBuffer() const;

    uint32_t getMaxRecursionDepth() const;
    void setMaxRecursionDepth(uint32_t max_recursion_depth);

    uint32_t getNumberOfScatteringIntegralImportanceDirections() const;
    uint32_t getNumberOfRays() const;
    uint32_t getNumberOfSpectraPairsSupported() const;

    float getRayMarchingStepSize() const;
    void setRayMarchingStepSize(float step_size);

    OxProgram getAbsorptionProbabilityShader() const;
    void setAbsorptionProbabilityShader(OxProgram const& absorption_probability_shader);

    OxProgram getScatteringProbabilityShader() const;
    void setScatteringProbabilityShader(OxProgram const& scattering_probability_shader);

    OxProgram getScatteringPhaseFunctionShader() const;
    void setScatteringPhaseFunctionShader(OxProgram const& scattering_phase_function_shader);

    OxMissShaderAssembly const& missShaderAssembly() const;
    OxMaterialAssembly const& materialAssembly() const;

private:
    uint32_t m_num_rays;
    uint32_t m_num_spectra_pairs_supported;
    uint32_t m_max_recursion_depth;
    float m_ray_marching_step_size;
    uint32_t m_num_scattering_integral_importance_directions;
    OxMaterialAssembly m_surface_material_assembly;
    OxMissShaderAssembly m_miss_shader_assembly;
    OxTraverseBackupBuffer m_traverse_backup_buffer;
    OxBuffer<float2> m_importance_directions_buffer;
};

}}

#endif
