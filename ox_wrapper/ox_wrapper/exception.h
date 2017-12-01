#ifndef OX_WRAPPER_EXCEPTION_H
#define OX_WRAPPER_EXCEPTION_H

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

        long line() const;
        std::string function() const;
        std::string file() const;

    private:
        long m_line;
        std::string m_function_name;
        std::string m_file_name;
    };

}


#endif
