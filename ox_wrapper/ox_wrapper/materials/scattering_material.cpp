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
    OxProgram const& scattering_phase_function_shader):
    OxContractWithOxContext{ absorption_probability_shader.context() },
    OxContractWithOxPrograms{ absorption_probability_shader, scattering_probability_shader, scattering_phase_function_shader },
    m_num_rays{ num_rays },
    m_num_spectra_pairs_supported{ num_spectra_pairs_supported },
    m_max_recursion_depth{ max_recursion_depth },
    m_ray_marching_step_size{ ray_marching_step_size },
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
}

float2* OxScatteringMaterial::mapImportanceDirectionsBuffer() const
{
    return static_cast<float2*>(m_importance_directions_buffer.map(OxBufferMapKind::read_write));
}

void materials::OxScatteringMaterial::unmapImportanceDirectionsBuffer() const
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
