#ifndef OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H
#define OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H

#include "buffer.h"

namespace ox_wrapper {

class OxTraverseBackupBuffer {
public:
    OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity);

    OxBuffer<unsigned int> getRawBuffer() const;

private:
    OxBuffer<unsigned int> m_raw_buffer;
};

}

#endif
