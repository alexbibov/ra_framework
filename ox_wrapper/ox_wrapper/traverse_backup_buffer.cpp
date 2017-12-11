#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ox_wrapper;

OxTraverseBackupBuffer::OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity):
    m_raw_buffer{ context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 9 + 1) }
{
    void* p_contents = m_raw_buffer.map(OxBufferMapKind::write);
    *reinterpret_cast<unsigned int*>(p_contents) = 0U;
    m_raw_buffer.unmap();
}

OxBuffer<unsigned int> OxTraverseBackupBuffer::getRawBuffer() const
{
    return m_raw_buffer;
}
