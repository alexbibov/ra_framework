#ifndef OX_WRAPPER_UTIL_MISC_H
#define OX_WRAPPER_UTIL_MISC_H

#include "../version.h"

#ifdef OX_WRAPPER_USE_CPU_INTRINSICS
#include <intrin.h>
#endif

#include "optional.h"
#include "static_vector.h"

namespace ox_wrapper { namespace util { namespace misc{

Optional<std::string> readAsciiTextFromSourceFile(std::string const& source_file);

std::string wstringToAsciiString(std::wstring const& wstr);

int8_t getFirstHighBit(uint64_t mask);

util::StaticVector<uint8_t, 64U> getSetBits(uint64_t mask);

}}}

#endif
