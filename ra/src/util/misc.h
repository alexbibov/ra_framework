#ifndef RA_UTIL_MISC_H
#define RA_UTIL_MISC_H

#include "../version.h"

#ifdef RA_USE_CPU_INTRINSICS
#include <intrin.h>
#endif

#include "optional.h"
#include "static_vector.h"

namespace ra { namespace util { namespace misc{

Optional<std::string> readAsciiTextFromSourceFile(std::string const& source_file);

std::string wstringToAsciiString(std::wstring const& wstr);

int8_t getFirstHighBit(uint64_t mask);

util::StaticVector<uint8_t, 64U> getSetBits(uint64_t mask);

bool doesFileExist(std::string const& file_path);

}}}

#endif
