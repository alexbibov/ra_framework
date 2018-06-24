#ifndef RA_RAY_GENERATOR_WITH_OUTPUT_BUFFER_H
#define RA_RAY_GENERATOR_WITH_OUTPUT_BUFFER_H

#include "ray_generator.h"
#include "ra/ray_casters/ra_ray_casters_fwd.h"

namespace ra {

class RaRayGeneratorWithOutputBuffer : public RaRayGenerator
{
public:
    RaRayGeneratorWithOutputBuffer(RaProgram const& ray_generation_shader, 
        RaAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);

    RaRayGeneratorWithOutputBuffer(RaProgram const& ray_generation_shader,
        RaMissShaderAssembly const& miss_shader_assembly,
        RaAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);

    RaAbstractBuffer const& outputBuffer() const override;

private:
    RaAbstractBuffer m_output_buffer;
};

}

#endif
