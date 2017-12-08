#ifndef OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H
#define OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H

#include "buffer.h"

namespace ox_wrapper {

class OxTraverseBackupBuffer {
public:
    OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity);

    OxBuffer<unsigned int> getRawBuffer() const;
    unsigned int getNumberOfStoredRays() const;
    float3 getRayOrigin(size_t ray_storage_index) const;
    float3 getRayDirection(size_t ray_storage_index) const;

private:
    OxBuffer<unsigned int> m_raw_buffer;
};

}

#endif
