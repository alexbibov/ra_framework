#ifndef OX_WRAPPER_EXCEPTION_H
#define OX_WRAPPER_EXCEPTION_H

#include "ox_wrapper/util/log.h"

#include <exception>
#include <cstdint>
#include <string>

namespace ox_wrapper {

    class OxException : public std::exception
    {
    public:
        OxException();
        OxException(char const* description);
        OxException(char const* description, 
            std::string const& file_name, 
            std::string const& function_name, 
            long line);
        OxException(std::string const& description);
        OxException(std::string const& description,
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


#define LOG_OX_WRAPPER_ERROR(err_msg)\
{ util::Log::retrieve()->out("ERROR: " + std::string{(err_msg)}\
    + "in function " + __FUNCTION__ ", line" + std::to_string(__LINE__)\
    + "(" + __FILE__ + ")", util::LogMessageType::error); }

#define THROW_OX_WRAPPER_ERROR(err_msg)\
{ util::Log::retrieve()->out("ERROR: " + std::string{(err_msg)}\
    + "in function " + __FUNCTION__ ", line" + std::to_string(__LINE__)\
    + "(" + __FILE__ + ")", util::LogMessageType::error);\
throw OxException{std::string{(err_msg)}, __FILE__, __FUNCTION__, __LINE__}; }


#endif
