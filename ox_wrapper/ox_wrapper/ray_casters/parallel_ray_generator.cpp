#include "parallel_ray_generator.h"
#include "../context.h"
#include "../../ptx.h"

using namespace ox_wrapper;
using namespace ox_wrapper::ray_casters;

OxParallelRayGenerator::OxParallelRayGenerator(OxContext const& context, uint32_t num_rays, float emitter_size, 
    float emitter_position, float emitter_rotation, uint32_t entry_point_index):
    OxRayGenerator{ context.createProgram(PTX_PARALLEL_RAY_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION), num_rays, 1U, 1U, entry_point_index },
    m_output_buffer{ context.createBuffer<OxRayRadiancePayload>(OxBufferKind::output, num_rays) },
    m_spectral_flux_buffer{ context.createBuffer<float>(OxBufferKind::input, 4 * OX_MAX_SPECTRA_QUADRUPLETS_SUPPORTED) }
{
    getRayGenerationShader().setVariableValue("num_rays", num_rays);
    setEmitterSize(emitter_size);
    setEmitterPosition(emitter_position);
    setEmitterRotation(emitter_rotation);
    getRayGenerationShader().assignBuffer("ox_output_buffer", m_output_buffer);
    getRayGenerationShader().assignBuffer("ox_init_spectral_flux_buffer", m_spectral_flux_buffer);
}

void OxParallelRayGenerator::setNumberOfRays(uint32_t num_rays)
{
    setGeneratorDimensions(num_rays, 1U, 1U);
    getRayGenerationShader().setVariableValue("num_rays", num_rays);
}

void OxParallelRayGenerator::setEmitterSize(float emitter_size)
{
    getRayGenerationShader().setVariableValue("emitter_size", emitter_size);
}

void OxParallelRayGenerator::setEmitterPosition(float emitter_position)
{
    getRayGenerationShader().setVariableValue("emitter_position", emitter_position);
}

void OxParallelRayGenerator::setEmitterRotation(float emitter_rotation)
{
    getRayGenerationShader().setVariableValue("emitter_rotation", emitter_rotation);
}

uint32_t OxParallelRayGenerator::getNumberOfRays() const
{
    uint32_t rv;
    getRayGenerationShader().getVariableValue("num_rays", &rv);
    return rv;
}

float OxParallelRayGenerator::getEmitterSize() const
{
    float rv;
    getRayGenerationShader().getVariableValue("emitter_size", &rv);
    return rv;
}

float OxParallelRayGenerator::getEmitterPosition() const
{
    float rv;
    getRayGenerationShader().getVariableValue("emitter_position", &rv);
    return rv;
}

float OxParallelRayGenerator::getEmitterRotation() const
{
    float rv;
    getRayGenerationShader().getVariableValue("emitter_rotation", &rv);
    return rv;
}
