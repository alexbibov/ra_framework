#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ra;

RaTraverseBackupBuffer::RaTraverseBackupBuffer(RaContext const& context, size_t max_ray_storage_capacity):
    m_current_buffer_idx{ new uint8_t{0U} },
    m_raw_buffer{ 
        context.createBuffer<unsigned int>(RaBufferKind::input_output, max_ray_storage_capacity * 9 + 1),
        context.createBuffer<unsigned int>(RaBufferKind::input_output, max_ray_storage_capacity * 9 + 1)
    }
{
    auto mapping = makeBufferMapSentry(m_raw_buffer[0], RaBufferMapKind::write);
    mapping.address()[0] = 0U;
}

RaTraverseBackupBuffer::~RaTraverseBackupBuffer()
{

}

RaBuffer<unsigned int> RaTraverseBackupBuffer::readBuffer() const
{
    return m_raw_buffer[(*m_current_buffer_idx) ^ 1];
}

RaBuffer<unsigned int> RaTraverseBackupBuffer::writeBuffer() const
{
    return m_raw_buffer[*m_current_buffer_idx];
}

bool RaTraverseBackupBuffer::isValid() const
{
    return m_raw_buffer[*m_current_buffer_idx].isValid();
}

void RaTraverseBackupBuffer::ping_pong() const
{
    *m_current_buffer_idx ^= 1;

    auto mapping = makeBufferMapSentry(m_raw_buffer[*m_current_buffer_idx], RaBufferMapKind::write);
    mapping.address()[0] = 0U;
}
