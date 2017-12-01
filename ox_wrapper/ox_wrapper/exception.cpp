#include "exception.h"
#include "../util/log.h"

using namespace ox_wrapper;

OxException::OxException():
    std::exception{ "Unknown OptiX wrapper exception" }
{
    util::Log const* p_log;
    if (p_log = util::Log::retrieve())
        p_log->out(what(), util::LogMessageType::error);
}

OxException::OxException(char const* description):
    std::exception{ description },
    m_line{ -1 },
    m_function_name{ "unknown" },
    m_file_name{ "unknown" }
{
    util::Log const* p_log;
    if (p_log = util::Log::retrieve())
    {
        p_log->out(what(), util::LogMessageType::error);
    }
}

OxException::OxException(char const* description, std::string const& file_name, std::string const& function_name, long line):
    std::exception{ description },
    m_line{ line },
    m_function_name{ function_name },
    m_file_name{ file_name }
{
    util::Log const* p_log;
    if (p_log = util::Log::retrieve())
    {
        p_log->out("ox_wrapper has thrown exception at line " + std::to_string(line) + " in function \""
            + function_name + "\" located at source file \"" + file_name + "\"", util::LogMessageType::error);
    }
}

long OxException::line() const
{
    return m_line;
}

std::string OxException::function() const
{
    return m_function_name;
}

std::string OxException::file() const
{
    return m_file_name;
}
