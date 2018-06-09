#ifndef OX_WRAPPER_UTIL_LUA_SUPPORT_H
#define OX_WRAPPER_UTIL_LUA_SUPPORT_H

#include "../../_3rd_party/sol2/sol.hpp"
#include "static_vector.h"


namespace ox_wrapper { namespace util { namespace lua_support {

template<typename ... Args>
struct Constructor
{
    using type = void(Args...);
};


template<typename T>
struct BaseClass
{
    using type = T;
};

struct ListOfBaseClasses
{
    template<typename ... Args>
    static sol::bases<typename Args::type...>
        make_initializer(Args...)
    {
        return sol::bases<typename Args::type...>{};
    }
};

struct ListOfConstructors
{
    template<typename ... Args>
    static sol::constructor_list<typename Args::type...>
        make_initializer(Args...)
    {
        return sol::constructor_list<typename Args::type...>{};
    }
};

struct ListOfFactories
{
    template<typename ... Args>
    static auto
        make_initializer(Args ... args)
    {
        return sol::factories(std::forward<Args>(args)...);
    }
};

struct NoConstructor
{
    static auto
        make_initializer()
    {
        return sol::no_constructor;
    }
};

struct ListOfOverloads
{
    template<typename ... Args>
    static auto
        make_initializer(Args ... args)
    {
        return sol::overload(std::forward<Args>(args)...);
    }
};

struct Property
{
    template<typename GetterType, typename SetterType>
    static auto
        make_initializer(GetterType getter, SetterType setter)
    {
        return sol::property(std::forward<GetterType>(getter), std::forward<SetterType>(setter));
    }
};

struct ReadOnlyProperty
{
    template<typename GetterType>
    static auto
        make_initializer(GetterType getter)
    {
        return sol::readonly(std::forward<GetterType>(getter));
    }
};

struct Variable
{
    template<typename T>
    static auto
        make_initializer(T variable)
    {
        return sol::var(std::forward<T>(variable));
    }
};

class LuaTable
{
    friend class LuaState;

public:
    
    using table_type = sol::table;


public:

    template<typename T>
    static std::vector<T> toVector(table_type const& table)
    {
        std::vector<T> rv{};
        rv.reserve(table.size());
        for (size_t i = 0; i < table.size(); ++i)
            rv.push_back(table[i + 1]);

        return rv;
    }

    template<typename T, size_t capacity>
    static util::StaticVector<T, capacity> toStaticVector(table_type const& table)
    {
        util::StaticVector<T, capacity> rv{};
        for (size_t i = 0; i < table.size(); ++i)
            rv.push_back(table[i + 1]);
        
        return rv;
    }

    template<typename T, size_t capacity>
    static table_type staticVectorToTable(StaticVector<T, capacity> const& static_vector)
    {
        table_type rv{};
        for (size_t i = 0; i < static_vector.size(); ++i)
            rv.set(i + 1, static_vector[i]);
        return rv;
    }

    template<typename T>
    static table_type vectorToTable(std::vector<T> const& vector)
    {
        table_type rv{};
        for (size_t i = 0; i < vector.size(); ++i)
            rv.set(i + 1, vector[i]);
        return rv;
    }


public:

    template<typename ... Args>
    void registerFunction(std::string const& name, Args const& ... args) 
    { 
        m_table.set_function(name, args...); 
    }

    template<typename RegistrantType, typename InitializerType, typename ... MemberArgs>
    LuaTable registerType(std::string const& name, InitializerType initializer, MemberArgs ... args)
    {
        sol::usertype<RegistrantType> utype{ "new", initializer, args... };
        table_type table = m_table.set_usertype(name, utype);
        return LuaTable{ std::move(table) };
    }

    template<typename RegistrantType, typename BasesType, typename InitializerType, typename ... MemberArgs>
    LuaTable registerSubType(std::string const& name, BasesType bases, InitializerType initializer, MemberArgs ... args)
    {
        sol::usertype<RegistrantType> utype{ "new", initializer, sol::base_classes, bases, args... };
        table_type table = m_table.set_usertype(name, utype) 
        return LuaTable{ std::move(table) };
    }

    template<typename ... MemberArgs>
    LuaTable registerEnum(std::string const& name, MemberArgs ... args)
    {
        table_type table = m_table.new_enum(name, std::forward<MemberArgs>(args)...);
        return LuaTable{ std::move(table) };
    }

    LuaTable registerTable(std::string const& name)
    {
        return LuaTable{ m_table.create_named(name) };
    }

private:
    LuaTable(table_type&& lua_table);

private:
    table_type m_table;
};


class LuaState
{
public:
    static void executeScript(std::string const& source_code);

    template<typename ... Args>
    static void registerFunction(std::string const& name, Args const& ... args)
    {
        initialize();
        m_lua_state->set_function(name, args...);

    }

    template<typename RegistrantType, typename InitializerType, typename ... MemberArgs>
    static LuaTable registerType(std::string const& name, InitializerType initializer, MemberArgs ... args)
    {
        initialize();
        sol::usertype<RegistrantType> utype{ "new", initializer, args... };
        m_lua_state->set_usertype(name, utype);
        
        sol::table table = (*m_lua_state)[name];
        return LuaTable{ std::move(table) };
    }

    template<typename RegistrantType, typename BasesType, typename InitializerType, typename ... MemberArgs>
    static LuaTable registerSubType(std::string const& name, BasesType bases, InitializerType initializer, MemberArgs ... args)
    {
        initialize();
        sol::usertype<RegistrantType> utype{ "new", initializer, sol::base_classes, bases, args... };
        m_lua_state->set_usertype(name, utype);

        sol::table table = (*m_lua_state)[name];
        return LuaTable{ std::move(table) };
    }
    
    template<typename ... MemberArgs>
    static LuaTable registerEnum(std::string const& name, MemberArgs ... args)
    {
        initialize();
        m_lua_state->new_enum(name, std::forward<MemberArgs>(args)...);

        sol::table table = (*m_lua_state)[name];
        return LuaTable{ std::move(table) };
    }

    static LuaTable registerTable(std::string const& name)
    {
        initialize();
        sol::table new_table = m_lua_state->create_named_table(name);
        return LuaTable{ std::move(new_table) };
    }

    static LuaTable retrieveTable(std::string const& name)
    {
        sol::table table = (*m_lua_state)[name];
        return LuaTable{ std::move(table) };
    }

    static void shutdown();

private:
    static void initialize();

private:
    static std::unique_ptr<sol::state> m_lua_state;
};


}}}


#endif
