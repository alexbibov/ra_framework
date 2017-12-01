#ifndef OX_WRAPPER_STATIC_VECTOR_H
#define OX_WRAPPER_STATIC_VECTOR_H

#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <cassert>

namespace ox_wrapper {

    template<typename T, size_t max_size>
    class StaticVector
    {
    public:
        class StaticVectorIterator
        {
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
            T& operator--();
            T operator--(int);

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
            StaticVectorIterator(char const* addr);

        private:
            char const* m_start;
            char const* m_addr;
        };

        using iterator_type = StaticVectorIterator;
        using const_iterator_type = StaticVectorIterator const;

        StaticVector();
        StaticVector(T const& val);
        StaticVector(std::initializer_list<T> elements);
        StaticVector(StaticVector const& other) = default;
        StaticVector(StaticVector&& other) = default;

        StaticVector& operator=(StaticVector const& other) = default;
        StaticVector& operator=(StaticVector&& other) = default;

        virtual ~StaticVector();

        T& operator[](size_t index);
        T const& operator[](size_t index) const;

        void push_back(T const& new_element);
        T pop_back();

        size_t size() const;
        size_t capacity() const;

        iterator_type begin() {}
        iterator_type end() {}

        const_iterator_type cbegin() const { return const_cast<StaticVector*>(this)->begin(); }
        const_iterator_type cend() const { return const_cast<StaticVector*>(this)->end(); }

        const_iterator_type begin() const { return cbegin(); }
        const_iterator_type end() const { return cend(); }

    private:
        template<typename = typename std::enable_if<std::is_class<T>::value && std::is_destructible<T>::value>::type>
        static void destruct_element(T* p_e) { p_e->~T(); }

        template<typename = typename std::enable_if<!std::is_class<T>::value || !std::is_destructible<T>::value>::type>
        static void destruct_element(T* p_e, int = 0) {}

    private:
        static const size_t m_capacity = sizeof(T)*max_size;
        char m_data[m_capacity];
        char* m_p_end;
        size_t m_size;
    };

    template<typename T, size_t max_size>
    inline StaticVector<T, max_size>::StaticVector():
        m_data{ 0 },
        m_p_end{ m_data },
        m_size{ 0U }
    {

    }

    template<typename T, size_t max_size>
    inline StaticVector<T, max_size>::StaticVector(T const& val) :
        m_data{ 0 },
        m_p_end{ m_data },
        m_size{ 0U }
    {
        while (m_size < max_size) push_back(val);
    }

    template<typename T, size_t max_size>
    inline StaticVector<T, max_size>::StaticVector(std::initializer_list<T> elements):
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
    inline StaticVector<T, max_size>::~StaticVector()
    {
        for (size_t i = 0; i < m_size; ++i)
            destruct_element(reinterpret_cast<T*>(&m_data[i * sizeof(T)]));
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
        T* e_addr = reinterpret_cast<T*>(m_p_end - sizeof(T));
        T rv{ *e_addr };
        destruct_element(e_addr);
        return rv;
    }

    template<typename T, size_t max_size>
    inline size_t StaticVector<T, max_size>::size() const
    {
        return m_size;
    }

    template<typename T, size_t max_size>
    inline size_t StaticVector<T, max_size>::capacity() const
    {
        return m_capacity;
    }

    
    // required by random-access iterator canonical implementation
    template<typename T, size_t max_size>
    inline typename StaticVector<T, max_size>::StaticVectorIterator operator+(size_t n, typename StaticVector<T, max_size>::iterator_type const& other)
    {
        return other + n;
    }

    template<typename T, size_t max_size>
    inline StaticVector<T, max_size>::StaticVectorIterator::StaticVectorIterator(StaticVectorIterator const& other):
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
        return !(*this == other);
    }
    template<typename T, size_t max_size>
    inline T* StaticVector<T, max_size>::StaticVectorIterator::operator->()
    {
        assert((m_addr - m_start < StaticVector<T, max_size>::m_capacity));
        return reinterpret_cast<T*>(m_addr);
    }
    template<typename T, size_t max_size>
    inline StaticVector<T, max_size>::StaticVectorIterator::StaticVectorIterator():
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
    inline T& StaticVector<T, max_size>::StaticVectorIterator::operator--()
    {
        m_addr -= sizeof(T);
        assert(m_addr >= m_start);
        return *this;
    }
    template<typename T, size_t max_size>
    inline T StaticVector<T, max_size>::StaticVectorIterator::operator--(int)
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
        assert((m_addr + n * sizeof(T) - m_start < StaticVector<T, max_size>::m_capacity));
        return StaticVectorIterator{ m_addr + n * sizeof(T) };
    }
    template<typename T, size_t max_size>
    inline typename StaticVector<T, max_size>::StaticVectorIterator StaticVector<T, max_size>::StaticVectorIterator::operator-(size_t n) const
    {
        assert(m_addr - n * sizeof(T) >= m_start);
        return StaticVectorIterator{ m_addr - n * sizeof(T) };
    }
    template<typename T, size_t max_size>
    inline size_t StaticVector<T, max_size>::StaticVectorIterator::operator-(StaticVectorIterator const& other) const
    {
        return reinterpret_cast<size_t>(m_addr - other.m_addr);
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
    inline StaticVector<T, max_size>::StaticVectorIterator::StaticVectorIterator(char const* addr) :
        m_start{ addr },
        m_addr{ addr }
    {
    }
}

#endif
