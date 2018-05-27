#ifndef OX_WRAPPER_LUA_LIBS_LUA_UTILITY_FUNCTIONS_LIBRARY_H
#define OX_WRAPPER_LUA_LIBS_LUA_UTILITY_FUNCTIONS_LIBRARY_H

#include "ox_wrapper/util/lua_support.h"

namespace ox_wrapper { namespace lua_libs {

class OxLuaDirectoryRoutinesRegistrar
{
public:
    OxLuaDirectoryRoutinesRegistrar(util::lua_support::LuaTable& parent_table);
};

class OxLuaLibsMainRegistrar
{
public:
    OxLuaLibsMainRegistrar();

private:
    std::unique_ptr<OxLuaDirectoryRoutinesRegistrar> m_directory_routines_registrar;
};

}}

#endif