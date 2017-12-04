#include "log.h"

#include <chrono>
#include <sstream>


using namespace ox_wrapper::util;

namespace
{
static thread_local Log const* p_thread_logger = nullptr;
}


Log const& Log::create(std::ostream& output_logging_stream, std::string const& log_name, int8_t time_zone /* = 0 */, bool is_dts /* = false */)
{
    if (!p_thread_logger)
    {
        p_thread_logger = new Log(output_logging_stream, time_zone, is_dts);

        std::string log_header_string{ log_name + " log output(" +
            DateTime::now(time_zone, is_dts).toString(DateTime::DateOutputMask::year |
                DateTime::DateOutputMask::month |
                DateTime::DateOutputMask::day) + ")" };

        output_logging_stream << "<!DOCTYPE html>" << std::endl;
        output_logging_stream << "<html>" << std::endl;
        output_logging_stream << "<head><title>" << log_header_string << "</title></head>" << std::endl;
        output_logging_stream << "<body>" << std::endl;
        output_logging_stream << "<table>" << std::endl;
        output_logging_stream << "<thead><tr>" << std::endl;
        output_logging_stream << "<th colspan=\"2\">" << log_header_string << "</th>" << std::endl;
        output_logging_stream << "</tr></thead>" << std::endl;
        output_logging_stream << "<tbody>" << std::endl;

        p_thread_logger->out("*****************Log started*****************", LogMessageType::information);
        return *p_thread_logger;
    }
    else
    {
        const_cast<Log*>(p_thread_logger)->m_out_streams.push_back(&output_logging_stream);
        return *p_thread_logger;
    }
}

Log const* Log::retrieve()
{
    return p_thread_logger;
}

bool Log::shutdown()
{
    if (!p_thread_logger) return false; // nothing to shutdown
    p_thread_logger->out("*****************End of the log*****************\n\n\n", LogMessageType::information);

    {
        std::stringstream sstream{};
        sstream << "</tbody>" << std::endl;
        sstream << "</table>" << std::endl;
        sstream << "</body>" << std::endl;
        sstream << "</html" << std::endl;

        // write terminating tags to every stream attached to the logger
        for (auto& out_stream : p_thread_logger->m_out_streams)
        {
            *out_stream << sstream.rdbuf()->str();
        }
    }

    delete p_thread_logger;
    p_thread_logger = nullptr;
    return true;
}


Log::Log(std::ostream& output_logging_stream, int8_t time_zone, bool is_dts) :
    m_time_stamp{ time_zone, is_dts }, m_out_streams{}, m_tabs{ 0 }
{
    m_out_streams.push_back(&output_logging_stream);
}


void Log::out(std::string const& message, LogMessageType message_type) const
{
    std::stringstream sstream{};

    sstream << "<tr bgcolor=\"";
    switch (message_type)
    {
    case LogMessageType::information:
        sstream << "#f2f2f2\">" << std::endl;
        break;
    case LogMessageType::exclamation:
        sstream << "#ffcc00\">" << std::endl;
        break;
    case LogMessageType::error:
        sstream << "#ff0000\">" << std::endl;
        break;
    }

    sstream << "<td align=\"center\">";
    //Generate preamble for the logging entry
    std::string month_name[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    m_time_stamp = DateTime::now(m_time_stamp.getTimeZone(), m_time_stamp.isDTS());
    sstream << "/" << m_time_stamp.toString() << "</td>";

    //Add message to the preamble
    sstream << "<td align=\"right\">";
    if (message_type == LogMessageType::error || message_type == LogMessageType::exclamation) sstream << "<b>";
    for (int i = 0; i < m_tabs - 1; ++i) sstream << "\t";
    sstream << message;
    if (message_type == LogMessageType::error || message_type == LogMessageType::exclamation) sstream << "</b>";
    sstream << "</td>" << std::endl << "</tr>";

    //Write entry to the log
    for (auto& out_stream : m_out_streams)
    {
        *out_stream << sstream.rdbuf()->str();
    }
}


DateTime const& Log::getLastEntryTimeStamp() const { return m_time_stamp; }



Log::scoped_tabulation_helper::scoped_tabulation_helper(Log& logger) : m_logger{ logger }
{
    ++m_logger.m_tabs;
}

Log::scoped_tabulation_helper::~scoped_tabulation_helper()
{
    if (m_logger.m_tabs > 0) --m_logger.m_tabs;
}
