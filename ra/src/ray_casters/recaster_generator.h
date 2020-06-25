#ifndef RA_RAY_CASTERS_RECASTER_GENERATOR_H
#define RA_RAY_CASTERS_RECASTER_GENERATOR_H

#include "../ray_generator_with_output_buffer.h"
#include "../traverse_backup_buffer.h"
#include "../ray_payloads.h"

namespace ra { namespace ray_casters {

class RaRecasterGenerator : public RaRayGeneratorWithOutputBuffer
{
public:
    RaRecasterGenerator(uint3 const& original_problem_size,
        RaTraverseBackupBuffer const& traverse_backup_buffer, 
        RaBuffer<RaRayRadiancePayload> const& output_buffer, 
        RaRayType recasted_ray_type, 
        float recasted_ray_parametric_length = 1.e27f);

    RaRecasterGenerator(uint3 const& original_problem_size,
        RaTraverseBackupBuffer const& traverse_backup_buffer, 
        RaBuffer<RaRayRadiancePayloadSimple> const& output_buffer, 
        RaRayType recasted_ray_type, 
        float recasted_ray_parametric_length = 1.e27f);

    RaRecasterGenerator(uint3 const& original_problem_size,
        RaTraverseBackupBuffer const& traverse_backup_buffer, 
        RaBuffer<RaRayRadiancePayloadMonochromatic> const& output_buffer, 
        RaRayType recasted_ray_type, 
        float recasted_ray_parametric_length = 1.e27f);

    RaRecasterGenerator(uint3 const& original_problem_size,
        RaTraverseBackupBuffer const& traverse_backup_buffer, 
        RaBuffer<RaRayOcclusionPayload> const& output_buffer, 
        RaRayType recasted_ray_type, 
        float recasted_ray_parametric_length = 1.e27f);

    void update(RaObjectHandle top_scene_object) const override;

    ~RaRecasterGenerator();

private:
    class impl;

    RaTraverseBackupBuffer m_traverse_backup_buffer;
    std::shared_ptr<impl> m_impl;
};

}}

#endif
