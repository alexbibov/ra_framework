#ifndef OX_WRAPPER_RAYCASTERS_RAYCASTERS_FACTORY_H
#define OX_WRAPPER_RAYCASTERS_RAYCASTERS_FACTORY_H

#include "ra_ray_casters_fwd.h"
#include "ra/ra_fwd.h"
#include "ra/context.h"
#include "ra/ray_payloads.h"

namespace ra { namespace ray_casters {

class RaRaycastersFactory final : public RaEntity
{
public:
    static RaRaycastersFactory* initialize(RaContext const& context);
    static void shutdown();
    static RaRaycastersFactory* retrieve();

public:
    // required by RaEntity interface
    bool isValid() const override;

    // RaParallelRayGenerator
public:
    RaParallelRayGenerator createParallelRayGenerator(uint32_t num_rays, float emitter_size,
        float emitter_position = 0.f, float emitter_rotation = 0.f,
        uint8_t num_spectra_pairs_supported = constants::max_spectra_pairs_supported,
        uint32_t entry_point_index = 0U) const;
    
    // RaRecasterGenerator
public:
    RaRecasterGenerator createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
        RaBuffer<RaRayRadiancePayload> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f) const;
    RaRecasterGenerator createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
        RaBuffer<RaRayRadiancePayloadSimple> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f) const;
    RaRecasterGenerator createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
        RaBuffer<RaRayRadiancePayloadMonochromatic> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f) const;
    RaRecasterGenerator createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
        RaBuffer<RaRayOcclusionPayload> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length = 1.e27f) const;

    RaTraverseBackupBuffer createTraverseBackupBuffer(size_t max_ray_storage_capacity);


private:
    RaRaycastersFactory(RaContext const& context);

private:
    RaContext const& m_context;
    
};


}}

#endif
