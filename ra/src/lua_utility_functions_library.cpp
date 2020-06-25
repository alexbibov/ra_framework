#include "lua_utility_functions_library.h"
#include "util/lua_support.h"
#include "util/misc.h"
#include <filesystem>

using namespace ra;
using namespace ra::lua_libs;
using namespace ra::util::lua_support;


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

RaLuaDirectoryRoutinesRegistrar::RaLuaDirectoryRoutinesRegistrar(LuaTable& parent_table)
{
    LuaTable this_lib_table = parent_table.registerTable("RaDirectoryLib");
    this_lib_table.registerFunction("get_current_directory", directory_functions::getCurrentDirectory);
}

RaLuaLibsMainRegistrar::RaLuaLibsMainRegistrar()
{
    LuaTable main_lib_table = LuaState::registerTable("RaLibs");
    m_directory_routines_registrar = std::make_unique<RaLuaDirectoryRoutinesRegistrar>(main_lib_table);
}
