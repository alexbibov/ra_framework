#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ox_wrapper;

OxTraverseBackupBuffer::OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity):
    m_raw_buffer{ context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 9 + 1) }
{
    
}

OxTraverseBackupBuffer::~OxTraverseBackupBuffer()
{

}

OxBuffer<unsigned int> OxTraverseBackupBuffer::getRawBuffer() const
{
    return m_raw_buffer;
}

bool OxTraverseBackupBuffer::isValid() const
{
    return m_raw_buffer.isValid();
}
