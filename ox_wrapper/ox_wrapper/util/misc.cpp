#include "misc.h"
#include "../exception.h"
#include "log.h"

#include <fstream>
#include <sstream>

using namespace ox_wrapper;
using namespace ox_wrapper::util;
using namespace ox_wrapper::util::misc;

Optional<std::string> ox_wrapper::util::misc::readAsciiTextFromSourceFile(std::string const& source_file)
{
    std::ifstream ifile{ source_file, std::ios::in };
    if (!ifile)
    {
        Log::retrieve()->out("ERROR: unable to read file \"" + source_file + "\"", LogMessageType::error);
        return Optional<std::string>{};
    }

    std::ostringstream ostrstream{};
    ostrstream << ifile.rdbuf();

    ifile.close();

    return Optional<std::string>{ostrstream.str()};
}

std::string ox_wrapper::util::misc::wstringToAsciiString(std::wstring const& wstr)
{
    size_t len = wstr.size();
    char* str = new char[len];

    for (size_t i = 0; i < len; ++i)
        str[i] = static_cast<char>(wstr[i]);

    std::string rv{ str, len };

    delete[] str;

    return rv;
}