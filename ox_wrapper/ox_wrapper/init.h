#ifndef OX_WRAPPER_INIT_H
#define OX_WRAPPER_INIT_H

#include "ox_wrapper/ox_wrapper_fwd.h"
#include "ox_wrapper/util/log.h"
#include "ox_wrapper/factory_initializer_sentinel.h"

#include <memory>
#include <fstream>

namespace ox_wrapper
{

class OxInit final
{
public:
    OxInit(
        std::string const& global_path_prefix,
        std::string const& path_to_settings);

    ~OxInit();
    
    OxContext& context() const;
    util::Log const& logger() const;
    std::string loggerPath() const;

    void executeLuaScriptFromSource(std::string const& lua_source_file) const;

private:
    std::unique_ptr<OxContext> m_context;
    std::string m_path_to_settings_json;
    std::string m_logging_path;

    std::ofstream m_logging_stream;

    std::unique_ptr<OxFactoryInitializerSentinel> m_factories_sentinel;
};

}

#endif
