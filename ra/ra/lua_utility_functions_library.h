#ifndef RA_LUA_LIBS_LUA_UTILITY_FUNCTIONS_LIBRARY_H
#define RA_LUA_LIBS_LUA_UTILITY_FUNCTIONS_LIBRARY_H

#include "ra/util/lua_support.h"

namespace ra { namespace lua_libs {

class RaLuaDirectoryRoutinesRegistrar
{
public:
    RaLuaDirectoryRoutinesRegistrar(util::lua_support::LuaTable& parent_table);
};

class RaLuaLibsMainRegistrar
{
public:
    RaLuaLibsMainRegistrar();

private:
    std::unique_ptr<RaLuaDirectoryRoutinesRegistrar> m_directory_routines_registrar;
};

}}

#endif