#ifndef OX_WRAPPER_UTIL_HASHED_STRING_H
#define OX_WRAPPER_UTIL_HASHED_STRING_H

#include <string>

#include "../../SpookyHash/SpookyV2.h"

namespace ox_wrapper{ namespace util{

class HashedString
{
private:

    // the numbers for the seed values are taken from www.random.org
    static uint64_t const m_seed1 = 0x0490c5d8f38f3c0e;
    static uint64_t const m_seed2 = 0x47af9b0d0436624f;
    static uint64_t const m_seed3 = 0x686e0851380250be;
    static thread_local spooky_hash_v2::SpookyHash m_hash_generator;


private:
    std::string m_string;
    uint64_t m_hash;

public:
    HashedString();
    HashedString(std::string const& str);

    bool operator<(HashedString const& other) const;
    bool operator==(HashedString const& other) const;

    uint64_t hash() const;
    char const* string() const;
};

}}

#endif
