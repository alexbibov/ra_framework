#include "traverse_backup_buffer.h"
#include "context.h"

using namespace ox_wrapper;

OxTraverseBackupBuffer::OxTraverseBackupBuffer(OxContext const& context, size_t max_ray_storage_capacity):
    m_raw_buffer{ context.createBuffer<unsigned int>(OxBufferKind::input_output, max_ray_storage_capacity * 24) }
{
    void* p_contents = m_raw_buffer.map(OxBufferMapKind::write);
    *reinterpret_cast<unsigned int*>(p_contents) = 0U;
    m_raw_buffer.unmap();
}

OxBuffer<unsigned int> OxTraverseBackupBuffer::getRawBuffer() const
{
    return m_raw_buffer;
}

unsigned int OxTraverseBackupBuffer::getNumberOfStoredRays() const
{
    unsigned int rv = *reinterpret_cast<unsigned int*>(m_raw_buffer.map(OxBufferMapKind::read));
    m_raw_buffer.unmap();
    return rv;
}

float3 OxTraverseBackupBuffer::getRayOrigin(size_t ray_storage_index) const
{
    unsigned int* p_contents = static_cast<unsigned int*>(m_raw_buffer.map(OxBufferMapKind::read));

    float3 rv;
    rv.x = *reinterpret_cast<float*>(p_contents + ray_storage_index * 6 + 0);
    rv.y = *reinterpret_cast<float*>(p_contents + ray_storage_index * 6 + 1);
    rv.z = *reinterpret_cast<float*>(p_contents + ray_storage_index * 6 + 2);
    m_raw_buffer.unmap();

    return rv;
}

float3 OxTraverseBackupBuffer::getRayDirection(size_t ray_storage_index) const
{
    unsigned int* p_contents = static_cast<unsigned int*>(m_raw_buffer.map(OxBufferMapKind::read));

    float3 rv;
    rv.x = *reinterpret_cast<float*>(p_contents + ray_storage_index * 6 + 3);
    rv.y = *reinterpret_cast<float*>(p_contents + ray_storage_index * 6 + 4);
    rv.z = *reinterpret_cast<float*>(p_contents + ray_storage_index * 6 + 5);
    m_raw_buffer.unmap();

    return rv;
}


