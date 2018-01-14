#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ox_wrapper;

OxTraverseBackupBuffer::OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity):
    m_raw_buffer{ context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 9 + 1) }
{
    m_p_buffer_contents = m_raw_buffer.map(OxBufferMapKind::read_write);
    m_p_buffer_contents[0] = 0U;
    // m_raw_buffer.unmap();
}

OxTraverseBackupBuffer::~OxTraverseBackupBuffer()
{
    m_raw_buffer.unmap();
}

OxBuffer<unsigned int> OxTraverseBackupBuffer::getRawBuffer() const
{
    return m_raw_buffer;
}

unsigned int* OxTraverseBackupBuffer::getBufferPointer() const
{
    return m_p_buffer_contents;
}

bool OxTraverseBackupBuffer::isValid() const
{
    return m_raw_buffer.isValid();
}
