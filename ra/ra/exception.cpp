#include "exception.h"
// #include "util/log.h"

using namespace ra;

RaException::RaException():
    std::exception{ "Unknown OptiX wrapper exception" }
{
    /*util::Log const* p_log;
    if (p_log = util::Log::retrieve())
        p_log->out(what(), util::LogMessageType::error);*/
}

RaException::RaException(char const* description):
    std::exception{ description },
    m_line{ -1 },
    m_function_name{ "unknown" },
    m_file_name{ "unknown" }
{
    /*util::Log const* p_log;
    if (p_log = util::Log::retrieve())
    {
        p_log->out(what(), util::LogMessageType::error);
    }*/
}

RaException::RaException(char const* description, std::string const& file_name, std::string const& function_name, long line):
    std::exception{ description },
    m_line{ line },
    m_function_name{ function_name },
    m_file_name{ file_name }
{
    /*util::Log const* p_log;
    if (p_log = util::Log::retrieve())
    {
        p_log->out("ra has thrown exception at line " + std::to_string(line) + " in function \""
            + function_name + "\" located at source file \"" + file_name + "\" (" + what() + ")", util::LogMessageType::error);
    }*/
}

RaException::RaException(std::string const& description):
    RaException{ description.c_str() }
{
}

RaException::RaException(std::string const& description, std::string const& file_name, std::string const& function_name, long line):
    RaException{ description.c_str(), file_name, function_name, line }
{
}

long RaException::line() const
{
    return m_line;
}

std::string RaException::function() const
{
    return m_function_name;
}

std::string RaException::file() const
{
    return m_file_name;
}
