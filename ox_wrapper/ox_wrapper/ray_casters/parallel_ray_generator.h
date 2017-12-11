#ifndef OX_WRAPPER_RAY_CASTERS_PARALLEL_RAY_GENERATOR_H
#define OX_WRAPPER_RAY_CASTERS_PARALLEL_RAY_GENERATOR_H

#include "../ray_generator.h"
#include "../buffer.h"
#include "../ray_radiance_payload.h"

namespace ox_wrapper { namespace ray_casters {

class OxParallelRayGenerator : public OxRayGenerator
{
public:
    OxParallelRayGenerator(OxContext const& context, uint32_t num_rays, 
        float emitter_size, float emitter_position = 0.f, float emitter_rotation = 0.f, uint32_t entry_point_index = 0U);

    void setNumberOfRays(uint32_t num_rays);
    void setEmitterSize(float emitter_size);
    void setEmitterPosition(float emitter_position);
    void setEmitterRotation(float emitter_rotation);

    uint32_t getNumberOfRays() const;
    float getEmitterSize() const;
    float getEmitterPosition() const;
    float getEmitterRotation() const;

private:
    OxBuffer<OxRayRadiancePayload> m_output_buffer;
    OxBuffer<float2> m_spectral_flux_buffer;
};

}}

#endif
