#ifndef OX_WRAPPER_RAY_GENERATOR_WITH_OUTPUT_BUFFER_H
#define OX_WRAPPER_RAY_GENERATOR_WITH_OUTPUT_BUFFER_H

#include "ray_generator.h"
#include "ra/ray_casters/ra_ray_casters_fwd.h"

namespace ra {

class OxRayGeneratorWithOutputBuffer : public OxRayGenerator
{
public:
    OxRayGeneratorWithOutputBuffer(OxProgram const& ray_generation_shader, 
        OxAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);

    OxRayGeneratorWithOutputBuffer(OxProgram const& ray_generation_shader,
        OxMissShaderAssembly const& miss_shader_assembly,
        OxAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U);

    OxAbstractBuffer const& outputBuffer() const override;

private:
    OxAbstractBuffer m_output_buffer;
};

}

#endif
