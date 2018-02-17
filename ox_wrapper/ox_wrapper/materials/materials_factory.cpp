#include "materials_factory.h"
#include "../util/lua_support.h"

using namespace ox_wrapper;
using namespace ox_wrapper::materials;
using namespace ox_wrapper::util;

namespace
{

static OxMaterialsFactory* p_factory_instance{ nullptr };

}

OxMaterialsFactory* OxMaterialsFactory::initialize(OxContext const& context)
{
    if (!p_factory_instance)
        p_factory_instance = new OxMaterialsFactory{ context };

    return p_factory_instance;
}

void OxMaterialsFactory::shutdown()
{
    if (p_factory_instance)
    {
        delete p_factory_instance;
        p_factory_instance = nullptr;
    }
}

OxMaterialsFactory* OxMaterialsFactory::retrieve()
{
    return p_factory_instance;
}

bool ox_wrapper::materials::OxMaterialsFactory::isValid() const
{
    return p_factory_instance;
}

OxBlackBody OxMaterialsFactory::createBlackBody(OxRayPayloadType payload_type, OxRayType affected_ray_type) const
{
    return OxBlackBody{ m_context, payload_type, affected_ray_type };
}

OxMaterialsFactory::OxMaterialsFactory(OxContext const& context) :
    m_context{ context }
{
    setStringName("OxMaterialsFactory");

    // register factory related types in Lua
    {
        lua_support::LuaState::registerSubType<OxBlackBody>(
            "OxBlackBody",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<OxEntity>{},
                lua_support::BaseClass<OxMaterial>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](OxRayPayloadType payload_type, OxRayType affected_ray_type) 
                {
                    return createBlackBody(payload_type, affected_ray_type); 
                }
            )
        );
    }
}
