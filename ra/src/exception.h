#ifndef RA_EXCEPTION_H
#define RA_EXCEPTION_H

#include "util/log.h"

#include <exception>
#include <cstdint>
#include <string>

namespace ra {

    class RaException : public std::exception
    {
    public:
        RaException();
        RaException(char const* description);
        RaException(char const* description, 
            std::string const& file_name, 
            std::string const& function_name, 
            long line);
        RaException(std::string const& description);
        RaException(std::string const& description,
            std::string const& file_name,
            std::string const& function_name,
            long line);

        long line() const;
        std::string function() const;
        std::string file() const;

    private:
        long m_line;
        std::string m_function_name;
        std::string m_file_name;
    };

}


#define LOG_RA_ERROR(err_msg)\
{ util::Log::retrieve()->out("ERROR: " + std::string{(err_msg)}\
    + "in function " + __FUNCTION__ ", line" + std::to_string(__LINE__)\
    + "(" + __FILE__ + ")", util::LogMessageType::error); }

#define THROW_RA_ERROR(err_msg)\
{ util::Log::retrieve()->out("ERROR: " + std::string{(err_msg)}\
    + "in function " + __FUNCTION__ ", line" + std::to_string(__LINE__)\
    + "(" + __FILE__ + ")", util::LogMessageType::error);\
throw RaException{std::string{(err_msg)}, __FILE__, __FUNCTION__, __LINE__}; }


#endif
