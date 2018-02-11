#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ox_wrapper;

OxTraverseBackupBuffer::OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity):
    m_raw_buffer{ context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 9 + 1) },
    m_persistent_map_sentinel{ m_raw_buffer, OxBufferMapKind::read_write }
{
    
}

OxTraverseBackupBuffer::~OxTraverseBackupBuffer()
{

}

OxBuffer<unsigned int> OxTraverseBackupBuffer::getRawBuffer() const
{
    return m_raw_buffer;
}

unsigned int* OxTraverseBackupBuffer::getBufferPointer() const
{
    return static_cast<unsigned int*>(m_persistent_map_sentinel.data());
}

bool OxTraverseBackupBuffer::isValid() const
{
    return m_raw_buffer.isValid();
}
