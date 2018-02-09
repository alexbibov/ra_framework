#include "shapes_factory.h"
#include "../util/lua_support.h"

using namespace ox_wrapper;
using namespace ox_wrapper::shapes;
using namespace ox_wrapper::util;

namespace {
static OxShapesFactory* p_shapes_factory_instance{ nullptr };
}

OxShapesFactory* OxShapesFactory::initialize(OxContext const& context)
{
    if (!p_shapes_factory_instance)
        p_shapes_factory_instance = new OxShapesFactory{ context };

    return p_shapes_factory_instance;
}

void OxShapesFactory::shutdown()
{
    if (p_shapes_factory_instance)
    {
        delete p_shapes_factory_instance;
        p_shapes_factory_instance = nullptr;
    }
}

OxShapesFactory* OxShapesFactory::retrieve()
{
    return p_shapes_factory_instance;
}

bool OxShapesFactory::isValid() const
{
    return p_shapes_factory_instance;
}

OxCircle OxShapesFactory::createCircle(float position_x, float position_y, float radius) const
{
    return OxCircle{ m_context, position_x, position_y, radius };
}

OxCircle OxShapesFactory::createCircle(OxMaterialAssembly const& material_assembly, float position_x, float position_y, float radius) const
{
    return OxCircle{ m_context, material_assembly, position_x, position_y, radius };
}

OxShapesFactory::OxShapesFactory(OxContext const& context):
    m_context{ context }
{
    setStringName("OxShapesFactory");

    // Register factory functions in Lua
    {

        lua_support::LuaState::registerSubType<OxCircle>(
            
            "OxCircle", 

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<OxEntity>{},
                lua_support::BaseClass<OxGeometry>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](float position_x, float position_y, float radius)
                {
                    return createCircle(position_x, position_y, radius);
                },
                [this](OxMaterialAssembly const& material_assembly, float position_x, float position_y, float radius)
                {
                    return createCircle(material_assembly, position_x, position_y, radius);
                }
            ),

            "updatePosition", 
            lua_support::ListOfOverloads::make_initializer(
                static_cast<void(OxCircle::*)(float2 const&)>(&OxCircle::updatePosition),
                static_cast<void(OxCircle::*)(float, float)>(&OxCircle::updatePosition)
            ),
            "updateRadius", &OxCircle::updateRadius,
            "getPosition", &OxCircle::getPosition,
            "getRadius", &OxCircle::getRadius);
    }
}
