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

int8_t ox_wrapper::util::misc::getFirstHighBit(uint64_t mask)
{
    int8_t rv{ -1 };

#ifdef OX_WRAPPER_USE_CPU_INTRINSICS
    unsigned long set_bit_index{};
    _BitScanReverse64(&set_bit_index, static_cast<unsigned long long>(mask));
    rv = static_cast<int8_t>(set_bit_index) - 1;
#else
    if (mask) ++rv;
    if (mask & 0xFFFFFFFF00000000) { rv += 32; mask &= 0xFFFFFFFF00000000; }
    if (mask & 0xFFFF0000FFFF0000) { rv += 16; mask &= 0xFFFF0000FFFF0000; }
    if (mask & 0xFF00FF00FF00FF00) { rv += 8; mask &= 0xFF00FF00FF00FF00; }
    if (mask & 0xF0F0F0F0F0F0F0F0) { rv += 4; mask &= 0xF0F0F0F0F0F0F0F0; }
    if (mask & 0xCCCCCCCCCCCCCCCC) { rv += 2; mask &= 0xCCCCCCCCCCCCCCCC; }
    if (mask & 0xAAAAAAAAAAAAAAAA) rv += 1;
#endif

    return rv;
}

ox_wrapper::util::StaticVector<uint8_t, 64U> ox_wrapper::util::misc::getSetBits(uint64_t mask)
{
    util::StaticVector<uint8_t, 64U> rv{};

    int8_t bit{};
    while (bit = getFirstHighBit(mask) >= 0)
    {
        rv.push_back(static_cast<uint8_t>(bit));
        mask ^= 0x1ui64 << bit;
    }

    return rv;
}





