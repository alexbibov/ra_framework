#include "shapes_factory.h"
#include "../util/lua_support.h"

using namespace ra;
using namespace ra::shapes;
using namespace ra::util;

namespace {
static RaShapesFactory* p_shapes_factory_instance{ nullptr };
}

RaShapesFactory* RaShapesFactory::initialize(RaContext const& context)
{
    if (!p_shapes_factory_instance)
        p_shapes_factory_instance = new RaShapesFactory{ context };

    return p_shapes_factory_instance;
}

void RaShapesFactory::shutdown()
{
    if (p_shapes_factory_instance)
    {
        delete p_shapes_factory_instance;
        p_shapes_factory_instance = nullptr;
    }
}

RaShapesFactory* RaShapesFactory::retrieve()
{
    return p_shapes_factory_instance;
}

bool RaShapesFactory::isValid() const
{
    return p_shapes_factory_instance;
}

RaCircle RaShapesFactory::createCircle(float position_x, float position_y, float radius) const
{
    return RaCircle{ m_context, position_x, position_y, radius };
}

RaCircle RaShapesFactory::createCircle(RaMaterialAssembly const& material_assembly, float position_x, float position_y, float radius) const
{
    return RaCircle{ m_context, material_assembly, position_x, position_y, radius };
}

RaShapesFactory::RaShapesFactory(RaContext const& context):
    m_context{ context }
{
    setStringName("RaShapesFactory");

    // Register factory functions in Lua
    {

        lua_support::LuaState::registerSubType<RaCircle>(
            
            "RaCircle", 

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{},
                lua_support::BaseClass<RaGeometry>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](float position_x, float position_y, float radius)
                {
                    return createCircle(position_x, position_y, radius);
                },
                [this](RaMaterialAssembly const& material_assembly, float position_x, float position_y, float radius)
                {
                    return createCircle(material_assembly, position_x, position_y, radius);
                }
            ),

            "updatePosition", 
            lua_support::ListOfOverloads::make_initializer(
                static_cast<void(RaCircle::*)(float2 const&)>(&RaCircle::updatePosition),
                static_cast<void(RaCircle::*)(float, float)>(&RaCircle::updatePosition)
            ),
            "updateRadius", &RaCircle::updateRadius,
            "getPosition", &RaCircle::getPosition,
            "getRadius", &RaCircle::getRadius);
    }
}
