#include "data_store_agents_factory.h"

#include "ox_wrapper/context.h"
#include "ox_wrapper/util/lua_support.h"

using namespace ox_wrapper;
using namespace ox_wrapper::data_store_agents;
using namespace ox_wrapper::util;

namespace {

static OxDataStoreAgentsFactory* p_data_store_agents_factory_instance{ nullptr };

}

OxDataStoreAgentsFactory* OxDataStoreAgentsFactory::initialize(OxContext const& context)
{
    if (!p_data_store_agents_factory_instance)
        p_data_store_agents_factory_instance = new OxDataStoreAgentsFactory(context);
    return p_data_store_agents_factory_instance;
}

void OxDataStoreAgentsFactory::shutdown()
{
    if (p_data_store_agents_factory_instance)
    {
        delete p_data_store_agents_factory_instance;
        p_data_store_agents_factory_instance = nullptr;
    }
}

OxDataStoreAgentsFactory* OxDataStoreAgentsFactory::retrieve()
{
    return p_data_store_agents_factory_instance;
}

bool OxDataStoreAgentsFactory::isValid() const
{
    return true;
}

OxMatlabV4 OxDataStoreAgentsFactory::createMatlabV4Agent(std::string const& target_path, bool append_data)
{
    return OxMatlabV4{ target_path, append_data };
}

OxDataStoreAgentsFactory::OxDataStoreAgentsFactory(OxContext const& context) :
    m_context{ context }
{
    setStringName("OxDataStoreAgentsFactory");

    // Register factory functions in Lua
    {
        lua_support::LuaState::registerSubType<OxMatlabV4>(
            "OxMatlabV4",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<OxEntity>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](std::string const& target_path)
                {
                    return createMatlabV4Agent(target_path);
                },

                [this](std::string const& target_path, bool append_data)
                {
                    return createMatlabV4Agent(target_path, append_data);
                }
            ),

            "save", &OxMatlabV4::save,
            "load", &OxMatlabV4::load
        );
    }
}
