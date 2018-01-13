#ifndef OX_WRAPPER_UTIL_MISC_H
#define OX_WRAPPER_UTIL_MISC_H

#include "optional.h"

namespace ox_wrapper { namespace util { namespace misc{

Optional<std::string> readAsciiTextFromSourceFile(std::string const& source_file);

std::string wstringToAsciiString(std::wstring const& wstr);

}}}

#endif
