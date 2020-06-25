#include "materials_factory.h"
#include "../util/lua_support.h"

using namespace ra;
using namespace ra::materials;
using namespace ra::util;

namespace
{

static RaMaterialsFactory* p_factory_instance{ nullptr };

}

RaMaterialsFactory* RaMaterialsFactory::initialize(RaContext const& context)
{
    if (!p_factory_instance)
        p_factory_instance = new RaMaterialsFactory{ context };

    return p_factory_instance;
}

void RaMaterialsFactory::shutdown()
{
    if (p_factory_instance)
    {
        delete p_factory_instance;
        p_factory_instance = nullptr;
    }
}

RaMaterialsFactory* RaMaterialsFactory::retrieve()
{
    return p_factory_instance;
}

bool ra::materials::RaMaterialsFactory::isValid() const
{
    return p_factory_instance;
}

RaBlackBody RaMaterialsFactory::createBlackBody(RaRayPayloadType payload_type, 
    RaRayTypeCollection const& affected_ray_types) const
{
    return RaBlackBody{ m_context, payload_type, affected_ray_types };
}

RaMaterialsFactory::RaMaterialsFactory(RaContext const& context) :
    m_context{ context }
{
    setStringName("RaMaterialsFactory");

    // register factory related types in Lua
    {
        lua_support::LuaState::registerSubType<RaBlackBody>(
            "RaBlackBody",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{},
                lua_support::BaseClass<RaMaterial>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](RaRayPayloadType payload_type, 
                    lua_support::LuaTable::table_type const& affected_ray_types) 
                {
                    return createBlackBody(payload_type, 
                        lua_support::LuaTable::toStaticVector<RaRayTypeCollection::value_type, RaRayTypeCollection::capacity()>(affected_ray_types)); 
                }
            )
        );
    }
}
