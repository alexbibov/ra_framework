#include "ox_exception.h"
#include "log.h"

using namespace ox_wrapper;

OxException::OxException():
    std::exception{ "Unknown OptiX wrapper exception" }
{
    Log const* p_log;
    if (p_log = Log::retrieve())
        p_log->out(what(), LogMessageType::error);
}

OxException::OxException(char const* description):
    std::exception{ description }
{
    Log const* p_log; ;
    if (p_log = Log::retrieve())
        p_log->out(what(), LogMessageType::error);
}
