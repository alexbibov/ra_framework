#include "parallel_ray_generator.h"
#include "../context.h"
#include "../../ptx.h"

#include <algorithm>

using namespace ox_wrapper;
using namespace ox_wrapper::ray_casters;

OxParallelRayGenerator::OxParallelRayGenerator(OxContext const& context, uint32_t num_rays, float emitter_size, 
    float emitter_position, float emitter_rotation, 
    uint8_t num_spectra_pairs_supported, uint32_t entry_point_index):
    OxRayGenerator{ context.createProgram(PTX_PARALLEL_RAY_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION), num_rays, 1U, 1U, entry_point_index },
    m_output_buffer{ context.createBuffer<OxRayRadiancePayload>(OxBufferKind::output, num_rays) },
    m_spectral_flux_buffer{ context.createBuffer<float2>(OxBufferKind::input, (std::min)(constants::max_spectra_pairs_supported, static_cast<uint32_t>(num_spectra_pairs_supported))) },
    m_num_spectra_pairs_supported{ num_spectra_pairs_supported }
{
    setGeneratorDimensions(num_rays, 1U, 1U);
    getRayGenerationShader().setVariableValue("num_rays", num_rays);
    getRayGenerationShader().setVariableValue("num_spectra_pairs_supported", num_spectra_pairs_supported);
    setEmitterSize(emitter_size);
    setEmitterPosition(emitter_position);
    setEmitterRotation(emitter_rotation);
    getRayGenerationShader().assignBuffer("ox_output_buffer", m_output_buffer);
    getRayGenerationShader().assignBuffer("ox_init_spectral_flux_buffer", m_spectral_flux_buffer);
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

uint8_t ray_casters::OxParallelRayGenerator::getNumberOfSpectraPairsSupported() const
{
    return m_num_spectra_pairs_supported;
}

float2* ox_wrapper::ray_casters::OxParallelRayGenerator::mapSpectralFluxBuffer() const
{
    return static_cast<float2*>(m_spectral_flux_buffer.map(OxBufferMapKind::read_write));
}

void ray_casters::OxParallelRayGenerator::unmapSpectralFluxBuffer() const
{
    m_spectral_flux_buffer.unmap();
}

OxRayRadiancePayload const* ray_casters::OxParallelRayGenerator::mapOutputbuffer() const
{
    return static_cast<OxRayRadiancePayload const*>(m_output_buffer.map(OxBufferMapKind::read));
}

void ray_casters::OxParallelRayGenerator::unmapOutputBuffer() const
{
    m_output_buffer.unmap();
}
