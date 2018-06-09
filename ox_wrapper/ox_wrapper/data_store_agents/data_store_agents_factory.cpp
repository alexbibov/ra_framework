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
        lua_support::LuaTable ox_matlab_v4_table = lua_support::LuaState::registerSubType<OxMatlabV4>(
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
            "load", &OxMatlabV4::load,
            "getVariables", &OxMatlabV4::getVariables
        );

        ox_matlab_v4_table.registerEnum("VariableDataFormat",
            "DOUBLE", OxMatlabV4::MatlabV4NumericDataFormat::double_precision_fp,
            "FLOAT", OxMatlabV4::MatlabV4NumericDataFormat::single_precision_fp,
            "INT32", OxMatlabV4::MatlabV4NumericDataFormat::signed_32bit_integer,
            "INT16", OxMatlabV4::MatlabV4NumericDataFormat::signed_16bit_integer,
            "UINT16", OxMatlabV4::MatlabV4NumericDataFormat::unsigned_16bit_integer,
            "UINT8", OxMatlabV4::MatlabV4NumericDataFormat::unsigned_8bit_integer);
        
        ox_matlab_v4_table.registerEnum("VariableDataType",
            "NUMERIC", OxMatlabV4::MatlabV4MatrixType::numeric,
            "TEXT", OxMatlabV4::MatlabV4MatrixType::text,
            "SPARSE", OxMatlabV4::MatlabV4MatrixType::sparse);

        ox_matlab_v4_table.registerType<OxMatlabV4::VariableInfo>(
            "VariableInfo",
            lua_support::NoConstructor::make_initializer(),
            "name", &OxMatlabV4::VariableInfo::name,
            "number_of_rows", &OxMatlabV4::VariableInfo::num_rows,
            "number_of_columns", &OxMatlabV4::VariableInfo::num_columns,
            "is_complex", &OxMatlabV4::VariableInfo::is_complex,
            "type", &OxMatlabV4::VariableInfo::type,
            "format", &OxMatlabV4::VariableInfo::format
            );

        
    }
}
