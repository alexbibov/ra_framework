#ifndef RA_INIT_H
#define RA_INIT_H

#include "ra_fwd.h"
#include "util/log.h"
#include "factory_initializer_sentinel.h"

#include <memory>
#include <fstream>

namespace ra
{

class RaInit final
{
public:
    RaInit(
        std::string const& global_path_prefix,
        std::string const& path_to_settings);

    ~RaInit();
    
    RaContext& context() const;
    util::Log const& logger() const;
    std::string loggerPath() const;

    void executeLuaScriptFromSource(std::string const& lua_source_file) const;

private:
    std::unique_ptr<RaContext> m_context;
    std::string m_path_to_settings_json;
    std::string m_logging_path;

    std::ofstream m_logging_stream;

    std::unique_ptr<RaFactoryInitializerSentinel> m_factories_sentinel;
};

}

#endif
