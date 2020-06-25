#include "data_store_agents_factory.h"

#include "context.h"
#include "util/lua_support.h"

using namespace ra;
using namespace ra::data_store_agents;
using namespace ra::util;

namespace {

static RaDataStoreAgentsFactory* p_data_store_agents_factory_instance{ nullptr };

}

RaDataStoreAgentsFactory* RaDataStoreAgentsFactory::initialize(RaContext const& context)
{
    if (!p_data_store_agents_factory_instance)
        p_data_store_agents_factory_instance = new RaDataStoreAgentsFactory(context);
    return p_data_store_agents_factory_instance;
}

void RaDataStoreAgentsFactory::shutdown()
{
    if (p_data_store_agents_factory_instance)
    {
        delete p_data_store_agents_factory_instance;
        p_data_store_agents_factory_instance = nullptr;
    }
}

RaDataStoreAgentsFactory* RaDataStoreAgentsFactory::retrieve()
{
    return p_data_store_agents_factory_instance;
}

bool RaDataStoreAgentsFactory::isValid() const
{
    return true;
}

RaMatlabV4 RaDataStoreAgentsFactory::createMatlabV4Agent(std::string const& target_path, bool append_data)
{
    return RaMatlabV4{ target_path, append_data };
}

RaDataStoreAgentsFactory::RaDataStoreAgentsFactory(RaContext const& context) :
    m_context{ context }
{
    setStringName("RaDataStoreAgentsFactory");

    // Register factory functions in Lua
    {
        lua_support::LuaTable ra_matlab_v4_table = lua_support::LuaState::registerSubType<RaMatlabV4>(
            "RaMatlabV4",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{}
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

            "save", &RaMatlabV4::save,
            "load", &RaMatlabV4::load,
            "getVariables", &RaMatlabV4::getVariables
        );

        ra_matlab_v4_table.registerEnum("VariableDataFormat",
            "DOUBLE", RaMatlabV4::MatlabV4NumericDataFormat::double_precision_fp,
            "FLOAT", RaMatlabV4::MatlabV4NumericDataFormat::single_precision_fp,
            "INT32", RaMatlabV4::MatlabV4NumericDataFormat::signed_32bit_integer,
            "INT16", RaMatlabV4::MatlabV4NumericDataFormat::signed_16bit_integer,
            "UINT16", RaMatlabV4::MatlabV4NumericDataFormat::unsigned_16bit_integer,
            "UINT8", RaMatlabV4::MatlabV4NumericDataFormat::unsigned_8bit_integer);
        
        ra_matlab_v4_table.registerEnum("VariableDataType",
            "NUMERIC", RaMatlabV4::MatlabV4MatrixType::numeric,
            "TEXT", RaMatlabV4::MatlabV4MatrixType::text,
            "SPARSE", RaMatlabV4::MatlabV4MatrixType::sparse);

        ra_matlab_v4_table.registerType<RaMatlabV4::VariableInfo>(
            "VariableInfo",
            lua_support::NoConstructor::make_initializer(),
            "name", &RaMatlabV4::VariableInfo::name,
            "number_of_rows", &RaMatlabV4::VariableInfo::num_rows,
            "number_of_columns", &RaMatlabV4::VariableInfo::num_columns,
            "is_complex", &RaMatlabV4::VariableInfo::is_complex,
            "type", &RaMatlabV4::VariableInfo::type,
            "format", &RaMatlabV4::VariableInfo::format
            );

        
    }
}
