#ifndef OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H
#define OX_WRAPPER_TRAVERSE_BACKUP_BUFFER_H

#include "buffer.h"

namespace ra {

class RaTraverseBackupBuffer : public RaEntity {
public:
    RaTraverseBackupBuffer(RaContext const& context, size_t max_ray_storage_capacity);
    ~RaTraverseBackupBuffer();

    RaBuffer<unsigned int> readBuffer() const;
    RaBuffer<unsigned int> writeBuffer() const;

    // required by RaEntity interface
    bool isValid() const override;

    void ping_pong() const;



private:
    std::shared_ptr<uint8_t> m_current_buffer_idx;
    RaBuffer<unsigned int> m_raw_buffer[2];    // traverse backup buffer is always a ping-pong buffer
};

}

#endif
