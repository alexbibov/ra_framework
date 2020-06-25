#include "entity.h"

using namespace ra;

thread_local uint64_t RaEntityId::m_id_counter = 0;


RaEntityId::RaEntityId() :
    m_owning_thread{ std::this_thread::get_id() },
    m_id{ ++m_id_counter }
{

}

bool RaEntityId::operator==(RaEntityId const& other) const
{
    return m_owning_thread == other.m_owning_thread
        && m_id == other.m_id;
}

bool RaEntityId::operator<(RaEntityId const& other) const
{
    return m_owning_thread < other.m_owning_thread
        || (m_owning_thread == other.m_owning_thread && m_id < other.m_id);
}

bool RaEntityId::operator<=(RaEntityId const& other) const
{
    return *this < other || *this == other;
}

bool RaEntityId::operator>(RaEntityId const& other) const
{
    return m_owning_thread > other.m_owning_thread
        || (m_owning_thread == other.m_owning_thread && m_id > other.m_id);
}

bool RaEntityId::operator>=(RaEntityId const& other) const
{
    return *this > other || *this == other;
}

bool RaEntityId::operator!=(RaEntityId const& other) const
{
    return !(*this == other);
}

size_t RaEntityId::owningThread() const
{
    std::hash<std::thread::id> hasher{};
    return hasher(m_owning_thread);
}

std::string RaEntityId::toString() const
{
    return std::to_string(owningThread()) + "_" + std::to_string(m_id);
}




thread_local uint64_t RaEntity::m_alive_entities = 0;

RaEntityId RaEntity::getUniqueIdentifier() const
{
    return m_id;
}

std::string RaEntity::getStringName() const
{
    return m_string_name;
}

void RaEntity::setStringName(std::string const& entity_string_name)
{
    m_string_name = entity_string_name;
}

uint64_t RaEntity::aliveEntities()
{
    return m_alive_entities;
}

RaEntity::RaEntity() :
    m_string_name{ "unnamed(id=" + m_id.toString() + ")" }
{
    ++m_alive_entities;
}

RaEntity::RaEntity(RaEntity const& other):
    m_id{ other.m_id },
    m_string_name{ other.m_string_name }
{
    ++m_alive_entities;
}

RaEntity::RaEntity(RaEntity&& other) : 
    m_id{ std::move(other.m_id) },
    m_string_name{ std::move(other.m_string_name) }
{
    ++m_alive_entities;
}

RaEntity& RaEntity::operator=(RaEntity const & other)
{
    if (this == &other)
        return *this;
    
    m_string_name = other.m_string_name;
    return *this;
}

RaEntity& RaEntity::operator=(RaEntity&& other)
{
    if (this == &other)
        return *this;

    m_string_name = std::move(other.m_string_name);
    return *this;
}

RaEntity::~RaEntity()
{
    --m_alive_entities;
}