#include "init.h"
#include "context.h"
#include "../../_3rd_party/json/json.hpp"
#include "util/misc.h"
#include "util/lua_support.h"

#include <windows.h>

#include <vector>

using namespace ox_wrapper;
using json = nlohmann::json;


namespace {

std::string correctPath(std::string const& path)
{
    std::string corrected_path{ path };
    if (path.length())
    {
        char last_symbol = path[path.length() - 1];
        if (last_symbol == '/' || last_symbol == '\\')
            corrected_path = std::string(path.begin(), path.end() - 1);

        if (corrected_path.length())
        {
            char first_symbol = corrected_path[0];
            if (first_symbol == '/' || first_symbol == '\\')
                corrected_path = std::string(corrected_path.begin() + 1, corrected_path.end());
        }
    }

    std::transform(corrected_path.begin(), corrected_path.end(), corrected_path.begin(),
        [](char e)->char
    {
        if (e == '\\')
            return '/';
        else
            return e;
    });

    return corrected_path;
}

}



OxInit::OxInit(
    std::string const& global_path_prefix,
    std::string const& path_to_settings)
{
    std::string corrected_global_path_prefix = correctPath(global_path_prefix);
    std::string corrected_path_to_settings = correctPath(path_to_settings);
    std::string full_path_to_settings = corrected_global_path_prefix + '/' + corrected_path_to_settings;

    // initialize system parameters
    m_logging_path = corrected_global_path_prefix + "/ox_wrapper.html";
    uint32_t num_entry_points = 1U;
    std::vector<std::string> asset_directories{};
    {
        util::Optional<std::string> settings_source = util::misc::readAsciiTextFromSourceFile(full_path_to_settings);
        json j;
        json::iterator p;

        if (settings_source.isValid())
        {
            j = json::parse(static_cast<std::string&>(settings_source));

            if ((p = j.find("logging_path")) != j.end()
                && p->is_string())
            {
                m_logging_path = corrected_global_path_prefix + '/' + correctPath(p->get<std::string>());
            }
        }


        // initialize logging
        {
            DYNAMIC_TIME_ZONE_INFORMATION time_zone_info;
            DWORD time_zone_id = GetDynamicTimeZoneInformation(&time_zone_info);
            bool dts = time_zone_id == 0 || time_zone_id == 2;
            int8_t time_zone_bias = static_cast<uint8_t>(-(time_zone_info.Bias + time_zone_info.StandardBias) / 60);

            m_logging_stream.open(m_logging_path, std::ios::out);
            if (!m_logging_stream)
            {
                throw std::runtime_error{ "unable to initialize logging stream" };
            }

            wchar_t host_computer_name[MAX_COMPUTERNAME_LENGTH + 1];
            DWORD host_computer_name_length;
            if (!GetComputerName(host_computer_name, &host_computer_name_length))
                wcscpy_s(host_computer_name, L"UNKNOWN_HOST");

            std::string log_name = "OX_WRAPPER \"" + util::misc::wstringToAsciiString(host_computer_name) + "\"";
            util::Log::create(m_logging_stream, log_name, time_zone_bias, dts);
        }


        if (settings_source.isValid())
        {
            if ((p = j.find("num_entry_points")) != j.end()
                && p->is_number_unsigned())
            {
                num_entry_points = *p;
            }
            else
            {
                util::Log::retrieve()->out("Unable to read entry \"num_entry_points\" while parsing "
                    "ox_wrapper JSON settings file \"" + path_to_settings + "\". The entry is either not found or is "
                    "having invalid format (unsigned number was expected)", util::LogMessageType::exclamation);
            }

            if ((p = j.find("asset_directories")) != j.end()
                && p->is_array())
            {
                for (auto& path : *p)
                {
                    if (path.is_string())
                    {
                        asset_directories.push_back(corrected_global_path_prefix + "/" + correctPath(path.get<std::string>()));
                    }
                    else
                    {
                        util::Log::retrieve()->out("Unable to parse asset directory from JSON array \"asset_directories\": "
                            "invalid entry format (string was expected)", util::LogMessageType::exclamation);
                    }
                }
            }
            else
            {
                util::Log::retrieve()->out("Unable to read entry \"asset_directories\" while parsing "
                    "ox_wrapper JSON settings file \"" + path_to_settings + "\". The entry is either not found or is "
                    "having invalid format (JSON array was expected)", util::LogMessageType::exclamation);
            }
        }
    }

    // create context
    {
        m_context.reset(new OxContext{ asset_directories, num_entry_points });
        m_factories_sentinel.reset(new OxFactoryInitializerSentinel{ *m_context });
        m_context->setStringName("OptiX context");
    }

    // register some auxiliary functions in Lua
    {
        util::lua_support::LuaState::registerFunction("ox_logger_path", [this]()->std::string {return loggerPath(); });
        util::lua_support::LuaState::registerFunction("ox_alive_entities", &OxEntity::aliveEntities);
    }
}

OxInit::~OxInit()
{
    // Lua must be shutdown first to make sure all ox_wrapper objects it has created are dead by the time the
    // context gets destroyed
    util::lua_support::LuaState::shutdown();    

    // the context and factories must be destroyed while the logger is still valid as errors may still occur during destruction
    m_factories_sentinel = nullptr;
    m_context = nullptr;
    auto alive_entities = OxEntity::aliveEntities();
    util::Log::retrieve()->out("Alive entities: " + std::to_string(alive_entities), 
        alive_entities ? util::LogMessageType::exclamation : util::LogMessageType::information);
    util::Log::shutdown();
    m_logging_stream.close();
}

OxContext const& OxInit::context() const
{
    return *m_context;
}

util::Log const& OxInit::logger() const
{
    return *util::Log::retrieve();
}

std::string OxInit::loggerPath() const
{
    return m_logging_path;
}

void OxInit::executeLuaScriptFromSource(std::string const& lua_source_file) const
{
    std::ifstream ifile{ lua_source_file, std::ios::in };
    std::string lua_source_script;
    if (ifile)
    {
        ifile.close();
        lua_source_script = static_cast<std::string>(util::misc::readAsciiTextFromSourceFile(lua_source_file));
    }
    else
    {
        lua_source_script = m_context->retrieveStringAsset(lua_source_file);
    }

    util::lua_support::LuaState::executeScript(lua_source_script);
}