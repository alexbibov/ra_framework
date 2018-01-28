#ifndef OX_WRAPPER_RAY_CASTERS_RECASTER_GENERATOR_H
#define OX_WRAPPER_RAY_CASTERS_RECASTER_GENERATOR_H

#include "../ray_generator_with_output_buffer.h"
#include "../traverse_backup_buffer.h"
#include "../ray_payloads.h"

namespace ox_wrapper {

class OxRecasterGenerator : public OxRayGeneratorWithOutputBuffer
{
public:
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayload> const& output_buffer, OxRayType recasted_ray_type);
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer, OxRayType recasted_ray_type);
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer, OxRayType recasted_ray_type);
    OxRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayOcclusionPayload> const& output_buffer, OxRayType recasted_ray_type);

    // required by OxRayGenerator interface
    OxAbstractBuffer const& outputBuffer() const override;

    void launch() const;

private:
    OxAbstractBuffer const& m_output_buffer;
};

}

#endif
