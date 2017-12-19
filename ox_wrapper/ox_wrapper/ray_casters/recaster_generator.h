#ifndef OX_WRAPPER_RAY_CASTERS_RECASTER_GENERATOR_H
#define OX_WRAPPER_RAY_CASTERS_RECASTER_GENERATOR_H

#include "../ray_generator.h"
#include "../traverse_backup_buffer.h"
#include "../ray_payloads.h"


namespace ox_wrapper {

class OxRecasterGenerator : public OxRayGenerator
{
public:
    OxRecasterGenerator(uint32_t num_total_rays, uint8_t dimension, OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayload> const& output_buffer);
    OxRecasterGenerator(uint32_t num_total_rays, uint8_t dimension, OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer);
    OxRecasterGenerator(uint32_t num_total_rays, uint8_t dimension, OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer);
    OxRecasterGenerator(uint32_t num_total_rays, uint8_t dimension, OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayOcclusionPayload> const& output_buffer);
};

}

#endif
