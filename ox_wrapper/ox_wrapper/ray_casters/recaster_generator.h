#ifndef OX_WRAPPER_RAY_CASTERS_RECASTER_GENERATOR_H
#define OX_WRAPPER_RAY_CASTERS_RECASTER_GENERATOR_H

#include "../ray_generator_with_output_buffer.h"
#include "../traverse_backup_buffer.h"
#include "../ray_payloads.h"

namespace ox_wrapper { namespace ray_casters {

class OxRecasterGenerator : public OxRayGeneratorWithOutputBuffer
{
public:
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayload> const& output_buffer, OxRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f);
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer, OxRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f);
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer, OxRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f);
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayOcclusionPayload> const& output_buffer, OxRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f);

    void update(OxObjectHandle top_scene_object) const override;

    ~OxRecasterGenerator();

private:
    class impl;

    OxTraverseBackupBuffer m_traverse_backup_buffer;
    std::shared_ptr<impl> m_impl;
};

}}

#endif
