#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ra;

OxTraverseBackupBuffer::OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity):
    m_current_buffer_idx{ new uint8_t{0U} },
    m_raw_buffer{ 
        context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 9 + 1),
        context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 9 + 1)
    }
{
    auto mapping = makeBufferMapSentry(m_raw_buffer[0], OxBufferMapKind::write);
    mapping.address()[0] = 0U;
}

OxTraverseBackupBuffer::~OxTraverseBackupBuffer()
{

}

OxBuffer<unsigned int> OxTraverseBackupBuffer::readBuffer() const
{
    return m_raw_buffer[(*m_current_buffer_idx) ^ 1];
}

OxBuffer<unsigned int> OxTraverseBackupBuffer::writeBuffer() const
{
    return m_raw_buffer[*m_current_buffer_idx];
}

bool OxTraverseBackupBuffer::isValid() const
{
    return m_raw_buffer[*m_current_buffer_idx].isValid();
}

void OxTraverseBackupBuffer::ping_pong() const
{
    *m_current_buffer_idx ^= 1;

    auto mapping = makeBufferMapSentry(m_raw_buffer[*m_current_buffer_idx], OxBufferMapKind::write);
    mapping.address()[0] = 0U;
}
