#ifndef OX_WRAPPER_ENTITY_H
#define OX_WRAPPER_ENTITY_H

#include <thread>
#include <string>

namespace ra{

class RaEntityId
{
public:
    RaEntityId();    //! initializes new identifier based on the calling thread and on the number of entities previously created by this thread

    bool operator==(RaEntityId const& other) const;
    bool operator<(RaEntityId const& other) const;
    bool operator<=(RaEntityId const& other) const;
    bool operator>(RaEntityId const& other) const;
    bool operator>=(RaEntityId const& other) const;
    bool operator!=(RaEntityId const& other) const;

    //! Returns numeric hash identifier of the thread that has created the object referred by the id
    size_t owningThread() const;

    //! Returns string representation of the identifier
    std::string toString() const;

private:
    static thread_local uint64_t m_id_counter;    //!< identifier counter local to each thread

    std::thread::id m_owning_thread;    //!< identifier of the thread that was used to create the object having this id
    uint64_t m_id;    //!< numeric identifier of the object
};


class RaEntity
{
public:
    RaEntityId getUniqueIdentifier() const;    //! returns unique identifier of the entity

    std::string getStringName() const;	//! returns user-friendly string name of the entity
    void setStringName(std::string const& entity_string_name);	//! sets new user-friendly string name for the entity

    static uint64_t aliveEntities();    //! returns number of alive entities owned by the calling thread

    virtual bool isValid() const = 0;    //! returns 'true' if the object exists in valid state (the state that is ready for ray tracing). Returns 'false' otherwise

    RaEntity();
    RaEntity(RaEntity const& other);
    RaEntity(RaEntity&& other);
    RaEntity& operator=(RaEntity const& other);
    RaEntity& operator=(RaEntity&& other);
    virtual ~RaEntity();

private:
    RaEntityId m_id;    //!< unique identifier of the entity
    std::string m_string_name;	//!< user-friendly string name of the entity
    static thread_local uint64_t m_alive_entities;    //!< number of alive entities owned by the calling thread
};

struct RaObjectHandle
{
    void* p_native = nullptr;
};

}

#endif