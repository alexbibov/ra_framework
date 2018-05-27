#include "lua_utility_functions_library.h"
#include "ox_wrapper/util/lua_support.h"
#include "ox_wrapper/util/misc.h"
#include <filesystem>

using namespace ox_wrapper;
using namespace ox_wrapper::lua_libs;
using namespace ox_wrapper::util::lua_support;


namespace {

namespace directory_functions
{

std::string getCurrentDirectory()
{
    auto cwd = std::filesystem::current_path();
    return util::misc::wstringToAsciiString(std::wstring{ cwd.c_str() });
}

}

}

OxLuaDirectoryRoutinesRegistrar::OxLuaDirectoryRoutinesRegistrar(LuaTable& parent_table)
{
    LuaTable this_lib_table = parent_table.registerTable("OxDirectoryLib");
    this_lib_table.registerFunction("get_current_directory", directory_functions::getCurrentDirectory);
}

OxLuaLibsMainRegistrar::OxLuaLibsMainRegistrar()
{
    LuaTable main_lib_table = LuaState::registerTable("OxLibs");
    m_directory_routines_registrar = std::make_unique<OxLuaDirectoryRoutinesRegistrar>(main_lib_table);
}