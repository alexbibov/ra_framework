#include "scattering_rendering_pass.h"
#include "context.h"
#include "ptx.h"
#include "scene_section.h"
#include "ray_generator.h"

#include "data_store_agents/matlab_v4.h"

using namespace ra;
using namespace ra::rendering_passes;

namespace {

void applyMaterialAssemblyToSceneSection(RaSceneSection& scene_section, RaMaterialAssembly const& material_assembly)
{
    for (auto& ss : scene_section.sceneSections())
        applyMaterialAssemblyToSceneSection(ss, material_assembly);

    bool scene_section_construction_was_delayed{ false };
    for (auto& gg : scene_section.geometryGroups())
    {
        bool material_assignment_was_delayed{ false };
        for (auto& g : gg.geometries())
        {
            if (static_cast<RaMaterialAssembly const&>(g.getMaterialAssembly()).isEmpty())
            {
                RaMaterialAssembly const& current_material_assembly = g.getMaterialAssembly();
                g.setMaterialAssembly(material_assembly);
                RaMaterialAssembly const& updated_material_assembly = g.getMaterialAssembly();
                material_assignment_was_delayed |= true;
            }
        }


        if (material_assignment_was_delayed)
        {
            scene_section_construction_was_delayed |= true;
            gg.endConstruction();

            if (!gg.isValid())
            {
                THROW_RA_ERROR("Geometry group \"" + gg.getStringName() + "\" is not valid after delayed material assignment");
            }
        }
    }

    if (scene_section_construction_was_delayed)
        scene_section.endConstruction();
}

}

RaScatteringRenderingPass::RaScatteringRenderingPass(
    RaSceneSection& scene_section,
    RaRayGenerator const& ray_caster,
    uint8_t num_spectra_pairs_supported,
    uint32_t max_recursion_depth,
    float ray_marching_step_size,
    uint32_t num_scattering_integral_importance_directions,
    RaProgram const& absorption_probability_shader,
    RaProgram const& scattering_probability_shader,
    RaProgram const& scattering_phase_function_shader)

    : RaRenderingPass{ scene_section }
    , RaContractWithRaPrograms{ absorption_probability_shader, scattering_probability_shader, scattering_phase_function_shader }
    , m_ray_caster{ ray_caster }
    , m_num_spectra_pairs_supported{ num_spectra_pairs_supported }
    //m_max_recursion_depth{ max_recursion_depth },
    //m_ray_marching_step_size{ ray_marching_step_size },
    , m_num_scattering_integral_importance_directions{ num_scattering_integral_importance_directions }
    , m_surface_material_assembly{
        RaMaterial{ targetSceneSection().context().createProgram(PTX_SCATTERING_RENDERING_PASS, RaProgram::Source::file, OX_SHADER_ENTRY_CLOSEST_HIT), util::Optional<RaProgram>{}, RaRayTypeCollection{ RaRayType::unknown } },
        RaMaterial{ targetSceneSection().context().createProgram(PTX_SCATTERING_RENDERING_PASS, RaProgram::Source::file, "__ra_closest_hit_scattered__"), util::Optional<RaProgram>{}, RaRayTypeCollection{ RaRayType::scattered }} }
    , m_miss_shader_assembly{
        RaMissShader{ targetSceneSection().context().createProgram(PTX_SCATTERING_RENDERING_PASS, RaProgram::Source::file, OX_SHADER_ENTRY_MISS), RaRayTypeCollection{ RaRayType::unknown } },
        RaMissShader{ targetSceneSection().context().createProgram(PTX_SCATTERING_RENDERING_PASS, RaProgram::Source::file, "__ra_miss_scattered__"), RaRayTypeCollection{ RaRayType::scattered } } }
    , m_traverse_backup_buffer{ targetSceneSection().context(), ray_caster.numberOfRays() }
    , m_importance_directions_buffer{ targetSceneSection().context().createBuffer<float2>(RaBufferKind::input, num_scattering_integral_importance_directions*(1 + num_spectra_pairs_supported)) }
    , m_recaster{ m_ray_caster.getGeneratorDimensions(), m_traverse_backup_buffer, castBufferToType<RaRayRadiancePayload>(m_ray_caster.outputBuffer()), RaRayType::unknown, ray_marching_step_size }
{
    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown)).getClosestHitShader())
        .setVariableValue("num_spectra_pairs_supported", m_num_spectra_pairs_supported);
    static_cast<RaProgram&>(static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).getProgram())
        .setVariableValue("num_spectra_pairs_supported", m_num_spectra_pairs_supported);

    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown)).getClosestHitShader())
        .setVariableValue("problem_size", m_ray_caster.getGeneratorDimensions());
    static_cast<RaProgram&>(static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).getProgram())
        .setVariableValue("problem_size", m_ray_caster.getGeneratorDimensions());

    setMaxRecursionDepth(max_recursion_depth);
    setRayMarchingStepSize(ray_marching_step_size);

    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown)).getClosestHitShader())
        .setVariableValue("num_importance_directions", num_scattering_integral_importance_directions);
    static_cast<RaProgram&>(static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).getProgram())
        .setVariableValue("num_importance_directions", num_scattering_integral_importance_directions);
    
    setAbsorptionProbabilityShader(absorption_probability_shader);
    setScatteringProbabilityShader(scattering_probability_shader);
    setScatteringPhaseFunctionShader(scattering_phase_function_shader);
    
    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown)).getClosestHitShader())
        .assignBuffer("importance_directions_buffer", m_importance_directions_buffer);
    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::scattered)).getClosestHitShader())
        .assignBuffer("importance_directions_buffer", m_importance_directions_buffer);
    static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown))
        .getProgram().assignBuffer("importance_directions_buffer", m_importance_directions_buffer);
}

RaScatteringRenderingPass::RaScatteringRenderingPass(
    RaSceneSection& scene_section,
    RaRayGenerator const& ray_caster,
    uint8_t num_spectra_pairs_supported, 
    uint32_t max_recursion_depth, 
    float ray_marching_step_size, 
    uint32_t num_scattering_integral_importance_directions):
    RaScatteringRenderingPass{ 
    scene_section, ray_caster, num_spectra_pairs_supported, max_recursion_depth, 
    ray_marching_step_size, num_scattering_integral_importance_directions,
    scene_section.context().createProgram(PTX_SCATTERING_RENDERING_PASS_DEFAULT_SHADER_CONFIG, RaProgram::Source::file, "__ra_scattering_default_absorption_factor__"),
    scene_section.context().createProgram(PTX_SCATTERING_RENDERING_PASS_DEFAULT_SHADER_CONFIG, RaProgram::Source::file, "__ra_scattering_default_scattering_factor__"),
    scene_section.context().createProgram(PTX_SCATTERING_RENDERING_PASS_DEFAULT_SHADER_CONFIG, RaProgram::Source::file, "__ra_scattering_default_phase_function__") }
{
}

float2* RaScatteringRenderingPass::mapImportanceDirectionsBuffer() const
{
    return m_importance_directions_buffer.map(RaBufferMapKind::read_write);
}

void RaScatteringRenderingPass::unmapImportanceDirectionsBuffer() const
{
    m_importance_directions_buffer.unmap();
}

uint32_t RaScatteringRenderingPass::getMaxRecursionDepth() const
{
    return m_max_recursion_depth;
}

void RaScatteringRenderingPass::setMaxRecursionDepth(uint32_t max_recursion_depth)
{
    m_max_recursion_depth = max_recursion_depth;

    for (auto& material : m_surface_material_assembly)
    {
        static_cast<RaProgram&>(material.getClosestHitShader())
            .setVariableValue("max_recursion_depth", max_recursion_depth);
    }

    for (auto& miss_shader : m_miss_shader_assembly)
    {
        miss_shader.getProgram().setVariableValue("max_recursion_depth", max_recursion_depth);
    }
}

uint32_t RaScatteringRenderingPass::getNumberOfScatteringIntegralImportanceDirections() const
{
    return m_num_scattering_integral_importance_directions;
}

uint32_t RaScatteringRenderingPass::getNumberOfSpectraPairsSupported() const
{
    return m_num_spectra_pairs_supported;
}

float RaScatteringRenderingPass::getRayMarchingStepSize() const
{
    return m_ray_marching_step_size;
}

void RaScatteringRenderingPass::setRayMarchingStepSize(float step_size)
{
    m_ray_marching_step_size = step_size;

    for (auto& ms : m_miss_shader_assembly)
    {
        ms.getProgram().setVariableValue("step_size", step_size);
    }

    static_cast<RaProgram&>(
        static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown))
        .getClosestHitShader())
        .setVariableValue("step_size", step_size);
}

RaProgram RaScatteringRenderingPass::getAbsorptionProbabilityShader() const
{
    return getRaProgramFromDeclarationOffset(0U);
}

void RaScatteringRenderingPass::setAbsorptionProbabilityShader(RaProgram const& absorption_probability_shader)
{
    replaceProgramAtOffset(absorption_probability_shader, 0U);

    for (auto& ms : m_miss_shader_assembly)
    {
        ms.getProgram().setVariableValue("absorption_factor", getRaProgramFromDeclarationOffset(0U).getId());
    }

    static_cast<RaProgram&>(
        static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown))
        .getClosestHitShader())
        .setVariableValue("absorption_factor", getRaProgramFromDeclarationOffset(0U).getId());
}

RaProgram RaScatteringRenderingPass::getScatteringProbabilityShader() const
{
    return getRaProgramFromDeclarationOffset(1U);
}

void RaScatteringRenderingPass::setScatteringProbabilityShader(RaProgram const& scattering_probability_shader)
{
    replaceProgramAtOffset(scattering_probability_shader, 1U);

    static_cast<RaProgram&>(
        static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown))
        .getClosestHitShader())
        .setVariableValue("scattering_factor", getRaProgramFromDeclarationOffset(1U).getId());

    static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).
        getProgram().setVariableValue("scattering_factor", getRaProgramFromDeclarationOffset(1U).getId());
}

RaProgram RaScatteringRenderingPass::getScatteringPhaseFunctionShader() const
{
    return getRaProgramFromDeclarationOffset(2U);
}

void RaScatteringRenderingPass::setScatteringPhaseFunctionShader(RaProgram const& scattering_phase_function_shader)
{
    replaceProgramAtOffset(scattering_phase_function_shader, 2U);

    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown))
        .getClosestHitShader())
        .setVariableValue("phase_function", getRaProgramFromDeclarationOffset(2U).getId());

    static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).
        getProgram().setVariableValue("phase_function", getRaProgramFromDeclarationOffset(2U).getId());
}

void RaScatteringRenderingPass::prepare_impl()
{
    applyMaterialAssemblyToSceneSection(targetSceneSection(), m_surface_material_assembly);
}

void RaScatteringRenderingPass::render_impl() const
{
    static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown)).getClosestHitShader())
        .assignBuffer("traverse_backup_buffer", m_traverse_backup_buffer.writeBuffer());

    static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).getProgram()
        .assignBuffer("traverse_backup_buffer", m_traverse_backup_buffer.writeBuffer());
    
    m_ray_caster.setMissShaderAssembly(m_miss_shader_assembly);

    targetSceneSection().trace(m_ray_caster);

    m_traverse_backup_buffer.ping_pong();

    unsigned int num_not_converged_rays =  
        *makeBufferMapSentry(m_traverse_backup_buffer.readBuffer(), RaBufferMapKind::read).address();

    uint32_t count{ 1U };
    while (num_not_converged_rays > 0)
    {
        static_cast<RaProgram&>(static_cast<RaMaterial&>(m_surface_material_assembly.getMaterialByRayType(RaRayType::unknown)).getClosestHitShader())
            .assignBuffer("traverse_backup_buffer", m_traverse_backup_buffer.writeBuffer());
        static_cast<RaMissShader&>(m_miss_shader_assembly.getMissShaderByRayType(RaRayType::unknown)).getProgram().
            assignBuffer("traverse_backup_buffer", m_traverse_backup_buffer.writeBuffer());


        targetSceneSection().trace(m_recaster);

        m_traverse_backup_buffer.ping_pong();
        
        num_not_converged_rays = 
            *makeBufferMapSentry(m_traverse_backup_buffer.readBuffer(), RaBufferMapKind::read).address();


        // exporter.save(m_ray_caster.outputBuffer(), 0, RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD, "debug_output_" + std::to_string(count));
        // exporter.save(m_traverse_backup_buffer.readBuffer(), 0, RaBasicBufferFormat::UINT, "debug_traverse_backup_" + std::to_string(count));

        ++count;
    }
}
