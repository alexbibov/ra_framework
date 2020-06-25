#include "parallel_ray_generator.h"
#include "context.h"
#include "ptx.h"

#include <algorithm>

using namespace ra;
using namespace ra::ray_casters;

RaParallelRayGenerator::RaParallelRayGenerator(RaContext const& context, uint32_t num_rays, float emitter_size, 
    float emitter_position, float emitter_rotation, 
    uint8_t num_spectra_pairs_supported, uint32_t entry_point_index):
    RaRayGeneratorWithOutputBuffer{ context.createProgram(PTX_PARALLEL_RAY_GENERATOR, RaProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    context.createBuffer<RaRayRadiancePayload>(RaBufferKind::output, num_rays), "ra_output_buffer", num_rays, 1U, 1U, entry_point_index },
    m_spectral_flux_buffer{ context.createBuffer<float2>(RaBufferKind::input, num_rays*(std::min)(constants::max_spectra_pairs_supported, static_cast<uint32_t>(num_spectra_pairs_supported))) },
    m_num_spectra_pairs_supported{ num_spectra_pairs_supported }
{
    setGeneratorDimensions(num_rays, 1U, 1U);
    getRayGenerationShader().setVariableValue("num_rays", num_rays);
    getRayGenerationShader().setVariableValue("num_spectra_pairs_supported", static_cast<uint32_t>(num_spectra_pairs_supported));
    setEmitterSize(emitter_size);
    setEmitterPosition(emitter_position);
    setEmitterRotation(emitter_rotation);
    getRayGenerationShader().assignBuffer("ra_init_flux_buffer", m_spectral_flux_buffer);
}


void RaParallelRayGenerator::setEmitterSize(float emitter_size)
{
    getRayGenerationShader().setVariableValue("emitter_size", emitter_size);
}

void RaParallelRayGenerator::setEmitterPosition(float emitter_position)
{
    getRayGenerationShader().setVariableValue("emitter_position", emitter_position);
}

void RaParallelRayGenerator::setEmitterRotation(float emitter_rotation)
{
    getRayGenerationShader().setVariableValue("emitter_rotation", emitter_rotation);
}

uint32_t RaParallelRayGenerator::getNumberOfRays() const
{
    uint32_t rv;
    getRayGenerationShader().getVariableValue("num_rays", rv);
    return rv;
}

float RaParallelRayGenerator::getEmitterSize() const
{
    float rv;
    getRayGenerationShader().getVariableValue("emitter_size", rv);
    return rv;
}

float RaParallelRayGenerator::getEmitterPosition() const
{
    float rv;
    getRayGenerationShader().getVariableValue("emitter_position", rv);
    return rv;
}

float RaParallelRayGenerator::getEmitterRotation() const
{
    float rv;
    getRayGenerationShader().getVariableValue("emitter_rotation", rv);
    return rv;
}

uint8_t ray_casters::RaParallelRayGenerator::getNumberOfSpectraPairsSupported() const
{
    return m_num_spectra_pairs_supported;
}

float2* ra::ray_casters::RaParallelRayGenerator::mapSpectralFluxBuffer() const
{
    return m_spectral_flux_buffer.map(RaBufferMapKind::read_write);
}

void ray_casters::RaParallelRayGenerator::unmapSpectralFluxBuffer() const
{
    m_spectral_flux_buffer.unmap();
}

RaRayRadiancePayload const* ray_casters::RaParallelRayGenerator::mapOutputbuffer() const
{
    return static_cast<RaRayRadiancePayload const*>(outputBuffer().map(RaBufferMapKind::read));
}

void ray_casters::RaParallelRayGenerator::unmapOutputBuffer() const
{
    outputBuffer().unmap();
}
