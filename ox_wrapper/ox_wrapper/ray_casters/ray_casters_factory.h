#ifndef OX_WRAPPER_RAYCASTERS_RAYCASTERS_FACTORY_H
#define OX_WRAPPER_RAYCASTERS_RAYCASTERS_FACTORY_H

#include "ox_wrapper_ray_casters_fwd.h"
#include "ox_wrapper/ox_wrapper_fwd.h"
#include "ox_wrapper/context.h"
#include "ox_wrapper/ray_payloads.h"

namespace ox_wrapper { namespace ray_casters {

class OxRaycastersFactory final : public OxEntity
{
public:
    static OxRaycastersFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxRaycastersFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

    // OxParallelRayGenerator
public:
    OxParallelRayGenerator createParallelRayGenerator(uint32_t num_rays, float emitter_size,
        float emitter_position = 0.f, float emitter_rotation = 0.f,
        uint8_t num_spectra_pairs_supported = constants::max_spectra_pairs_supported,
        uint32_t entry_point_index = 0U) const;
    
    // OxRecasterGenerator
public:
    OxRecasterGenerator createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
        OxBuffer<OxRayRadiancePayload> const& output_buffer, OxRayType recasted_ray_type) const;
    OxRecasterGenerator createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
        OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer, OxRayType recasted_ray_type) const;
    OxRecasterGenerator createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
        OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer, OxRayType recasted_ray_type) const;
    OxRecasterGenerator createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
        OxBuffer<OxRayOcclusionPayload> const& output_buffer, OxRayType recasted_ray_type) const;

    OxTraverseBackupBuffer createTraverseBackupBuffer(size_t max_ray_storage_capacity);


private:
    OxRaycastersFactory(OxContext const& context);

private:
    OxContext const& m_context;
    
};


}}

#endif
