#ifndef RA_UTIL_STATIC_VECTOR_H
#define RA_UTIL_STATIC_VECTOR_H

#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <cassert>
#include <iterator>

namespace ra { namespace util {

template<typename T, size_t max_size>
class StaticVector
{
public:
    class StaticVectorIterator : public std::iterator<std::random_access_iterator_tag, T>
    {
        friend class StaticVector<T, max_size>;

    public:

        // required by output iterator canonical implementation

        StaticVectorIterator(StaticVectorIterator const& other);
        T& operator*();
        StaticVectorIterator& operator++();
        StaticVectorIterator operator++(int);

        // required by input iterator canonical implementation
        T const& operator*() const;
        T const* operator->() const;
        bool operator == (StaticVectorIterator const& other) const;
        bool operator != (StaticVectorIterator const& other) const;

        // required by forward iterator canonical implementation
        T* operator->();
        StaticVectorIterator();
        StaticVectorIterator& operator=(StaticVectorIterator const& other);

        // required by bidirectional iterator canonical implementation
        StaticVectorIterator& operator--();
        StaticVectorIterator operator--(int);

        // required by random-access iterator canonical implementation
        T& operator[](size_t index);
        T const& operator[](size_t index) const;
        StaticVectorIterator& operator+=(size_t n);
        StaticVectorIterator& operator-=(size_t n);
        StaticVectorIterator operator+(size_t n) const;
        StaticVectorIterator operator-(size_t n) const;
        size_t operator-(StaticVectorIterator const& other) const;
        bool operator<(StaticVectorIterator const& other) const;
        bool operator>(StaticVectorIterator const& other) const;
        bool operator<=(StaticVectorIterator const& other) const;
        bool operator>=(StaticVectorIterator const& other) const;

    private:
        StaticVectorIterator(char* addr);

    private:
        char* m_start;
        char* m_addr;
    };

    using value_type = T;

    using iterator = StaticVectorIterator;
    using const_iterator = StaticVectorIterator const;

    StaticVector();
    StaticVector(size_t num_elements, T const& val);
    StaticVector(size_t num_elements);
    StaticVector(std::initializer_list<T> elements);
    StaticVector(StaticVector const& other);
    StaticVector(StaticVector&& other);

    StaticVector& operator=(StaticVector const& other);
    StaticVector& operator=(StaticVector&& other);

    virtual ~StaticVector();

    T& operator[](size_t index);
    T const& operator[](size_t index) const;

    void push_back(T const& new_element);

    template<typename ...Args>
    void emplace_back(Args... args);

    T pop_back();

    size_t size() const;
    static constexpr size_t capacity() { return m_capacity / sizeof(T); }

    StaticVectorIterator find(T const& value) const;

    iterator begin() { return StaticVectorIterator{ m_data }; }
    iterator end() { return StaticVectorIterator{ m_p_end }; }

    const_iterator cbegin() const { return const_cast<StaticVector*>(this)->begin(); }
    const_iterator cend() const { return const_cast<StaticVector*>(this)->end(); }

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }

private:
    template<typename = typename std::enable_if<std::is_destructible<T>::value && !std::is_trivially_destructible<T>::value>::type>
    static void destruct_element(T* p_e) { p_e->~T(); }

    template<typename = typename std::enable_if<!std::is_destructible<T>::value || std::is_trivially_destructible<T>::value>::type>
    static void destruct_element(T* p_e, int = 0) {}

private:
    static constexpr size_t m_capacity = sizeof(T)*max_size;
    char m_data[m_capacity];
    char* m_p_end;
    size_t m_size;
};

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVector() :
    m_data{ 0 },
    m_p_end{ m_data },
    m_size{ 0U }
{

}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVector(size_t num_elements, T const& val) :
    m_data{ 0 },
    m_p_end{ m_data },
    m_size{ 0U }
{
    while (m_size < num_elements) push_back(val);
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVector(size_t num_elements) :
    m_data{ 0 },
    m_p_end{ m_data },
    m_size{ 0U }
{
    while (m_size < num_elements) push_back(T{});
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVector(std::initializer_list<T> elements) :
    m_data{ 0 },
    m_p_end{ m_data },
    m_size{ 0U }
{
    for (auto const& e : elements)
    {
        if (m_size < max_size) push_back(e);
        else break;
    }
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVector(StaticVector const& other):
    m_data{ 0 },
    m_p_end{ m_data },
    m_size{ 0U }
{
    for (auto& e : other)
    {
        push_back(e);
    }
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVector(StaticVector&& other):
    m_data{ 0 },
    m_p_end{ m_data },
    m_size{ 0U }
{
    for (auto& e : other)
    {
        emplace_back(std::move(e));
    }
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>& StaticVector<T, max_size>::operator=(StaticVector const& other)
{
    if (this == &other)
        return *this;

    while (m_size > other.m_size) pop_back();
    for (size_t i = 0; i < m_size; ++i)
    {
        (*this)[i] = other[i];
    }
    for (size_t i = m_size; i < other.m_size; ++i)
        push_back(other[i]);

    return *this;
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>& StaticVector<T, max_size>::operator=(StaticVector&& other)
{
    if (this == &other)
        return this;

    while (m_size > other.m_size) pop_back();
    for (size_t i = 0; i < m_size; ++i)
    {
        (*this)[i] = std::move(other[i]);
    }
    for (size_t i = m_size; i < other.m_size; ++i)
        emplace_back(std::move(other[i]));

    return *this;
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::~StaticVector()
{
    while (m_size > 0) pop_back();
}

template<typename T, size_t max_size>
inline T& StaticVector<T, max_size>::operator[](size_t index)
{
    assert(index < m_size);
    return *reinterpret_cast<T*>(&m_data[sizeof(T)*index]);
}

template<typename T, size_t max_size>
inline T const& StaticVector<T, max_size>::operator[](size_t index) const
{
    return const_cast<StaticVector<T, max_size>*>(this)->operator[](index);
}

template<typename T, size_t max_size>
inline void StaticVector<T, max_size>::push_back(T const& new_element)
{
    assert(m_size < max_size);
    new (m_p_end) T{ new_element };

    m_p_end += sizeof(T);
    ++m_size;
}

template<typename T, size_t max_size>
inline T StaticVector<T, max_size>::pop_back()
{
    m_p_end -= sizeof(T);
    T* e_addr = reinterpret_cast<T*>(m_p_end);
    T rv{ *e_addr };
    destruct_element(e_addr);
    --m_size;
    return rv;
}

template<typename T, size_t max_size>
inline size_t StaticVector<T, max_size>::size() const
{
    return m_size;
}

template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator StaticVector<T, max_size>::find(T const& value) const
{
    for (char* addr = m_data; addr < m_data + sizeof(T)*m_size; addr += sizeof(T))
    {
        if (*reinterpret_cast<T*>(addr) == value)
            return StaticVectorIterator{ addr };
    }

    return end();
}


// required by random-access iterator canonical implementation
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator operator+(size_t n, 
    typename StaticVector<T, max_size>::iterator const& other)
{
    return other + n;
}

template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVectorIterator::StaticVectorIterator(StaticVectorIterator const& other) :
    m_start{ other.m_start },
    m_addr{ other.m_addr }
{

}
template<typename T, size_t max_size>
inline T& StaticVector<T, max_size>::StaticVectorIterator::operator*()
{
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return *reinterpret_cast<T*>(m_addr);
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator& StaticVector<T, max_size>::StaticVectorIterator::operator++()
{
    m_addr += sizeof(T);
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return *this;
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator StaticVector<T, max_size>::StaticVectorIterator::operator++(int)
{
    StaticVectorIterator rv{ *this };
    m_addr += sizeof(T);
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return rv;
}
template<typename T, size_t max_size>
inline T const& StaticVector<T, max_size>::StaticVectorIterator::operator*() const
{
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return *reinterpret_cast<T*>(m_addr);
}
template<typename T, size_t max_size>
inline T const* StaticVector<T, max_size>::StaticVectorIterator::operator->() const
{
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return reinterpret_cast<T*>(m_addr);
}
template<typename T, size_t max_size>
inline bool StaticVector<T, max_size>::StaticVectorIterator::operator==(StaticVectorIterator const& other) const
{
    return m_addr == other.m_addr;
}
template<typename T, size_t max_size>
inline bool StaticVector<T, max_size>::StaticVectorIterator::operator!=(StaticVectorIterator const& other) const
{
    return m_addr != other.m_addr;
}
template<typename T, size_t max_size>
inline T* StaticVector<T, max_size>::StaticVectorIterator::operator->()
{
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return reinterpret_cast<T*>(m_addr);
}
template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVectorIterator::StaticVectorIterator() :
    m_start{ nullptr },
    m_addr{ nullptr }
{
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator& StaticVector<T, max_size>::StaticVectorIterator::operator=(StaticVectorIterator const& other)
{
    m_start = other.m_start;
    m_addr = other.m_addr;
    return *this;
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator& StaticVector<T, max_size>::StaticVectorIterator::operator--()
{
    m_addr -= sizeof(T);
    assert(m_addr >= m_start);
    return *this;
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator StaticVector<T, max_size>::StaticVectorIterator::operator--(int)
{
    StaticVectorIterator rv{ *this };
    m_addr -= sizeof(T);
    assert(m_addr >= m_start);
    return rv;
}
template<typename T, size_t max_size>
inline T& StaticVector<T, max_size>::StaticVectorIterator::operator[](size_t index)
{
    char* element_addr = m_addr + index * sizeof(T);
    assert((element_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return *reinterpret_cast<T*>(element_addr);
}
template<typename T, size_t max_size>
inline T const& StaticVector<T, max_size>::StaticVectorIterator::operator[](size_t index) const
{
    return (*const_cast<StaticVectorIterator*>(this))[index];
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator& StaticVector<T, max_size>::StaticVectorIterator::operator+=(size_t n)
{
    m_addr += n * sizeof(T);
    assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
    return *this;
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator& StaticVector<T, max_size>::StaticVectorIterator::operator-=(size_t n)
{
    m_addr -= n * sizeof(T);
    assert(m_addr >= m_start);
    return *this;
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator StaticVector<T, max_size>::StaticVectorIterator::operator+(size_t n) const
{
    char* ptr = m_addr + n * sizeof(T);
    assert((ptr - m_start < StaticVector<T, max_size>::m_capacity));
    return StaticVectorIterator{ ptr };
}
template<typename T, size_t max_size>
inline typename StaticVector<T, max_size>::StaticVectorIterator StaticVector<T, max_size>::StaticVectorIterator::operator-(size_t n) const
{
    char* ptr = m_addr - n * sizeof(T);
    assert(ptr >= m_start);
    return StaticVectorIterator{ ptr };
}
template<typename T, size_t max_size>
inline size_t StaticVector<T, max_size>::StaticVectorIterator::operator-(StaticVectorIterator const& other) const
{
    return reinterpret_cast<size_t>((m_addr - other.m_addr) / sizeof(T));
}
template<typename T, size_t max_size>
inline bool StaticVector<T, max_size>::StaticVectorIterator::operator<(StaticVectorIterator const& other) const
{
    return m_addr < other.m_addr;
}
template<typename T, size_t max_size>
inline bool StaticVector<T, max_size>::StaticVectorIterator::operator>(StaticVectorIterator const& other) const
{
    return m_addr > other.m_addr;
}
template<typename T, size_t max_size>
inline bool StaticVector<T, max_size>::StaticVectorIterator::operator<=(StaticVectorIterator const& other) const
{
    return m_addr <= other.m_addr;
}
template<typename T, size_t max_size>
inline bool StaticVector<T, max_size>::StaticVectorIterator::operator>=(StaticVectorIterator const& other) const
{
    return m_addr >= other.m_addr;
}
template<typename T, size_t max_size>
inline StaticVector<T, max_size>::StaticVectorIterator::StaticVectorIterator(char* addr) :
    m_start{ addr },
    m_addr{ addr }
{
}

template<typename T, size_t max_size>
template<typename ...Args>
inline void util::StaticVector<T, max_size>::emplace_back(Args ...args)
{
    assert(m_size < max_size);
    new (m_p_end) T{ std::forward<Args>(args)... };

    m_p_end += sizeof(T);
    ++m_size;
}


}}

#endif
