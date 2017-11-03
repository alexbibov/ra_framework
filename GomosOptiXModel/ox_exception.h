#ifndef OX_EXCEPTION_H

#include <exception>


namespace ox_wrapper {

    class OxException : public std::exception
    {
    public:
        OxException();
        OxException(char const* description);
    };

}

#define OX_EXCEPTION_H
#endif
