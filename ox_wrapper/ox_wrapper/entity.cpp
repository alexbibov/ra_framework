#include "entity.h"

using namespace ox_wrapper;

thread_local uint64_t OxEntityId::m_id_counter = 0;


OxEntityId::OxEntityId() :
    m_owning_thread{ std::this_thread::get_id() },
    m_id{ ++m_id_counter }
{

}

bool OxEntityId::operator==(OxEntityId const& other) const
{
    return m_owning_thread == other.m_owning_thread
        && m_id == other.m_id;
}

bool OxEntityId::operator<(OxEntityId const& other) const
{
    return m_owning_thread < other.m_owning_thread
        || (m_owning_thread == other.m_owning_thread && m_id < other.m_id);
}

bool OxEntityId::operator<=(OxEntityId const& other) const
{
    return *this < other || *this == other;
}

bool OxEntityId::operator>(OxEntityId const& other) const
{
    return m_owning_thread > other.m_owning_thread
        || (m_owning_thread == other.m_owning_thread && m_id > other.m_id);
}

bool OxEntityId::operator>=(OxEntityId const& other) const
{
    return *this > other || *this == other;
}

bool OxEntityId::operator!=(OxEntityId const& other) const
{
    return !(*this == other);
}

size_t OxEntityId::owningThread() const
{
    std::hash<std::thread::id> hasher{};
    return hasher(m_owning_thread);
}

std::string OxEntityId::toString() const
{
    return std::to_string(owningThread()) + "_" + std::to_string(m_id);
}




thread_local uint64_t OxEntity::m_alive_entities = 0;

OxEntityId OxEntity::getUniqueIdentifier() const
{
    return m_id;
}

std::string OxEntity::getStringName() const
{
    return m_string_name;
}

void OxEntity::setStringName(std::string const& entity_string_name)
{
    m_string_name = entity_string_name;
}

uint64_t OxEntity::aliveEntities()
{
    return m_alive_entities;
}

OxEntity::OxEntity() :
    m_string_name{ "unnamed(id=" + m_id.toString() + ")" }
{
    ++m_alive_entities;
}

OxEntity::OxEntity(OxEntity const& other):
    m_id{ other.m_id },
    m_string_name{ other.m_string_name }
{
    ++m_alive_entities;
}

OxEntity::OxEntity(OxEntity&& other) : 
    m_id{ std::move(other.m_id) },
    m_string_name{ std::move(other.m_string_name) }
{
    ++m_alive_entities;
}

OxEntity& OxEntity::operator=(OxEntity const & other)
{
    if (this == &other)
        return *this;
    
    m_string_name = other.m_string_name;
    return *this;
}

OxEntity& OxEntity::operator=(OxEntity&& other)
{
    if (this == &other)
        return *this;

    m_string_name = std::move(other.m_string_name);
    return *this;
}

OxEntity::~OxEntity()
{
    --m_alive_entities;
}