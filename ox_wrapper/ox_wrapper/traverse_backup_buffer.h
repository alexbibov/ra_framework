#ifndef OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H
#define OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H

#include "buffer.h"

namespace ox_wrapper {

class OxTraverseBackupBuffer : public OxEntity {
public:
    OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity);
    ~OxTraverseBackupBuffer();

    OxBuffer<unsigned int> getRawBuffer() const;

    unsigned int* getBufferPointer() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    OxBuffer<unsigned int> m_raw_buffer;
    unsigned int* m_p_buffer_contents;
};

}

#endif
