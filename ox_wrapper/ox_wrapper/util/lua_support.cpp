#include "lua_support.h"
#include "../exception.h"

using namespace ox_wrapper::util::lua_support;

std::unique_ptr<sol::state> LuaState::m_lua_state{ nullptr };


void LuaState::executeScript(std::string const& source_code)
{
    initialize();

    try
    {
        m_lua_state->script(source_code);
    }
    catch (sol::error& e)
    {
        throw OxException{ e.what(), __FILE__, __FUNCTION__, __LINE__ };
    }
}

void LuaState::shutdown()
{
    if (m_lua_state)
        m_lua_state = nullptr;
}

void LuaState::initialize()
{
    if (!m_lua_state)
    {
        m_lua_state.reset(new sol::state{});
        m_lua_state->open_libraries(sol::lib::base, sol::lib::math, sol::lib::io, sol::lib::os);
    }
}