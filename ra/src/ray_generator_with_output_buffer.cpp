#include "ray_generator_with_output_buffer.h"

using namespace ra;

RaRayGeneratorWithOutputBuffer::RaRayGeneratorWithOutputBuffer(
    RaProgram const& ray_generation_shader, 
    RaAbstractBuffer const& output_buffer,
    std::string const& output_buffer_binding_name, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, 
    uint32_t entry_point_index) : 
    RaRayGenerator{ ray_generation_shader, num_rays_x, num_rays_y, num_rays_z, entry_point_index },
    m_output_buffer{ output_buffer }
{
    getRayGenerationShader().assignBuffer(output_buffer_binding_name, output_buffer);
}

ra::RaRayGeneratorWithOutputBuffer::RaRayGeneratorWithOutputBuffer(
    RaProgram const& ray_generation_shader, 
    RaMissShaderAssembly const& miss_shader_assembly,
    RaAbstractBuffer const& output_buffer, 
    std::string const& output_buffer_binding_name, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, 
    uint32_t entry_point_index):
    RaRayGenerator{ ray_generation_shader, miss_shader_assembly, num_rays_x, num_rays_y, num_rays_z, entry_point_index },
    m_output_buffer{ output_buffer }
{
    getRayGenerationShader().assignBuffer(output_buffer_binding_name, output_buffer);
}

RaAbstractBuffer const& RaRayGeneratorWithOutputBuffer::outputBuffer() const
{
    return m_output_buffer;
}