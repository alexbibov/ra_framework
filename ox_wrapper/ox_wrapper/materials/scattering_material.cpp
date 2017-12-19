#include "scattering_material.h"
#include "../context.h"
#include "../../ptx.h"

using namespace ox_wrapper;
using namespace ox_wrapper::materials;

OxScatteringMaterial::OxScatteringMaterial(
    uint32_t num_rays,
    uint8_t num_spectra_pairs_supported,
    uint32_t max_recursion_depth,
    float ray_marching_step_size,
    uint32_t num_scattering_integral_importance_directions,
    OxProgram const& absorption_probability_shader,
    OxProgram const& scattering_probability_shader,
    OxProgram const& scattering_phase_function_shader) :
    OxContractWithOxContext{ absorption_probability_shader.context() },
    OxContractWithOxPrograms{ absorption_probability_shader, scattering_probability_shader, scattering_phase_function_shader },
    m_num_rays{ num_rays },
    m_num_spectra_pairs_supported{ num_spectra_pairs_supported },
    //m_max_recursion_depth{ max_recursion_depth },
    //m_ray_marching_step_size{ ray_marching_step_size },
    m_num_scattering_integral_importance_directions{ num_scattering_integral_importance_directions },
    m_surface_material_assembly{
    OxMaterial{context().createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, OX_SHADER_ENTRY_CLOSEST_HIT), OxRayType::unknown},
    OxMaterial{context().createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, "__ox_intersect_scattered__"), OxRayType::scattered} },
    m_miss_shader_assembly{
    OxMissShader{context().createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, OX_SHADER_ENTRY_MISS), OxRayType::unknown},
    OxMissShader{context().createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, "__ox_miss_scattered__"), OxRayType::scattered} },
    m_traverse_backup_buffer{ context(), m_num_rays },
    m_importance_directions_buffer{ context().createBuffer<float2>(OxBufferKind::input, num_scattering_integral_importance_directions*(1 + num_spectra_pairs_supported)) }
{
    static_cast<OxMaterial&>(m_surface_material_assembly.getMaterialByRayType(OxRayType::unknown)).getClosestHitShader().declareVariable("num_spectra_pairs_supported", m_num_spectra_pairs_supported);
    static_cast<OxMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(OxRayType::unknown)).getMissShader().declareVariable("num_spectra_pairs_supported", m_num_spectra_pairs_supported);

    setMaxRecursionDepth(max_recursion_depth);
    setRayMarchingStepSize(ray_marching_step_size);

    static_cast<OxMaterial&>(m_surface_material_assembly.getMaterialByRayType(OxRayType::unknown)).getClosestHitShader().declareVariable("num_importance_directions", num_scattering_integral_importance_directions);
    static_cast<OxMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(OxRayType::unknown)).getMissShader().declareVariable("num_importance_directions", num_scattering_integral_importance_directions);
    
    setAbsorptionProbabilityShader(absorption_probability_shader);
    setScatteringProbabilityShader(scattering_probability_shader);
    setScatteringPhaseFunctionShader(scattering_phase_function_shader);
}

OxScatteringMaterial::OxScatteringMaterial(
    OxContext const& context,
    uint32_t num_rays, 
    uint8_t num_spectra_pairs_supported, 
    uint32_t max_recursion_depth, 
    float ray_marching_step_size, 
    uint32_t num_scattering_integral_importance_directions):
    OxScatteringMaterial{ num_rays, num_spectra_pairs_supported, max_recursion_depth, ray_marching_step_size, num_scattering_integral_importance_directions,
                          context.createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, "__ox_scattering_material_default_absorption_factor__"),
                          context.createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, "__ox_scattering_material_default_scattering_factor__"),
                          context.createProgram(PTX_SCATTERING_MATERIAL, OxProgram::Source::file, "__ox_scattering_material_default_phase_funciton__") }

{
}

float2* OxScatteringMaterial::mapImportanceDirectionsBuffer() const
{
    return static_cast<float2*>(m_importance_directions_buffer.map(OxBufferMapKind::read_write));
}

void OxScatteringMaterial::unmapImportanceDirectionsBuffer() const
{
    m_importance_directions_buffer.unmap();
}

uint32_t OxScatteringMaterial::getMaxRecursionDepth() const
{
    return m_max_recursion_depth;
}

void OxScatteringMaterial::setMaxRecursionDepth(uint32_t max_recursion_depth)
{
    m_max_recursion_depth = max_recursion_depth;

    for (auto& material : m_surface_material_assembly)
    {
        material.getClosestHitShader().declareVariable("max_recursion_depth", max_recursion_depth);
    }

    for (auto& miss_shader : m_miss_shader_assembly)
    {
        miss_shader.getMissShader().declareVariable("max_recursion_depth", max_recursion_depth);
    }
}

uint32_t OxScatteringMaterial::getNumberOfScatteringIntegralImportanceDirections() const
{
    return m_num_scattering_integral_importance_directions;
}

uint32_t OxScatteringMaterial::getNumberOfRays() const
{
    return m_num_rays;
}

uint32_t OxScatteringMaterial::getNumberOfSpectraPairsSupported() const
{
    return m_num_spectra_pairs_supported;
}

float OxScatteringMaterial::getRayMarchingStepSize() const
{
    return m_ray_marching_step_size;
}

void OxScatteringMaterial::setRayMarchingStepSize(float step_size)
{
    for (auto& ms : m_miss_shader_assembly)
    {
        ms.getMissShader().declareVariable("step_size", m_ray_marching_step_size);
    }

    static_cast<OxMaterial&>(m_surface_material_assembly.getMaterialByRayType(OxRayType::unknown)).
        getClosestHitShader().declareVariable("step_size", m_ray_marching_step_size);
}

OxProgram OxScatteringMaterial::getAbsorptionProbabilityShader() const
{
    return getOxProgramFromDeclarationOffset(0U);
}

void OxScatteringMaterial::setAbsorptionProbabilityShader(OxProgram const& absorption_probability_shader)
{
    replaceProgramAtOffset(absorption_probability_shader, 0U);

    for (auto& ms : m_miss_shader_assembly)
    {
        ms.getMissShader().declareVariable("absorption_factor", OxObjectHandle{ nativeOptiXProgramHandle(0U) });
    }

    static_cast<OxMaterial&>(m_surface_material_assembly.getMaterialByRayType(OxRayType::unknown)).
        getClosestHitShader().declareVariable("absorption_factor", OxObjectHandle{ nativeOptiXProgramHandle(0U) });
}

OxProgram OxScatteringMaterial::getScatteringProbabilityShader() const
{
    return getOxProgramFromDeclarationOffset(1U);
}

void OxScatteringMaterial::setScatteringProbabilityShader(OxProgram const& scattering_probability_shader)
{
    replaceProgramAtOffset(scattering_probability_shader, 1U);

    static_cast<OxMaterial&>(m_surface_material_assembly.getMaterialByRayType(OxRayType::unknown)).
        getClosestHitShader().declareVariable("scattering_factor", OxObjectHandle{ nativeOptiXProgramHandle(1U) });
    static_cast<OxMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(OxRayType::unknown)).
        getMissShader().declareVariable("scattering_factor", OxObjectHandle{ nativeOptiXProgramHandle(1U) });
}

OxProgram OxScatteringMaterial::getScatteringPhaseFunctionShader() const
{
    return getOxProgramFromDeclarationOffset(2U);
}

void OxScatteringMaterial::setScatteringPhaseFunctionShader(OxProgram const& scattering_phase_function_shader)
{
    replaceProgramAtOffset(scattering_phase_function_shader, 2U);

    static_cast<OxMaterial&>(m_surface_material_assembly.getMaterialByRayType(OxRayType::unknown)).
        getClosestHitShader().declareVariable("phase_function", OxObjectHandle{ nativeOptiXProgramHandle(2U) });
    static_cast<OxMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(OxRayType::unknown)).
        getMissShader().declareVariable("phase_function", OxObjectHandle{ nativeOptiXProgramHandle(2U) });
}

OxMissShaderAssembly const& OxScatteringMaterial::missShaderAssembly() const
{
    return m_miss_shader_assembly;
}

OxMaterialAssembly const& OxScatteringMaterial::materialAssembly() const
{
    return m_surface_material_assembly;
}
