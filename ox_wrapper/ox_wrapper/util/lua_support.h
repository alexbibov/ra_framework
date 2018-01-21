#ifndef OX_WRAPPER_UTIL_LUA_SUPPORT_H
#define OX_WRAPPER_UTIL_LUA_SUPPORT_H

#include "../../_3rd_party/sol2/sol.hpp"


namespace ox_wrapper { namespace util { namespace lua_support {

template<typename ... Args>
struct Constructor
{
    using type = void(Args...);
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
    static void registerType(std::string const& name, InitializerType initializer, MemberArgs ... args)
    {
        initialize();
        sol::usertype<RegistrantType> utype{ "new", initializer, args... };
        m_lua_state->set_usertype(name, utype);
    }
    
    template<typename ... MemberArgs>
    static void registerEnum(std::string const& name, MemberArgs ... args)
    {
        initialize();
        m_lua_state->new_enum(name, std::forward<MemberArgs>(args)...);
    }

    static void shutdown();

private:
    static void initialize();

private:
    static std::unique_ptr<sol::state> m_lua_state;
};


}}}


#endif
