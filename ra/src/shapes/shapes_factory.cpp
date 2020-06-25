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

RaCircle RaShapesFactory::createCircle(float location_x, float location_y, float radius) const
{
    return RaCircle{ m_context, location_x, location_y, radius };
}

RaCircle RaShapesFactory::createCircle(RaMaterialAssembly const& material_assembly, float location_x, float location_y, float radius) const
{
    return RaCircle{ m_context, material_assembly, location_x, location_y, radius };
}

RaSphere RaShapesFactory::createSphere(float location_x, float location_y, float location_z, float radius) const
{
    return createSphere(float3{ location_x, location_y, location_z }, radius);
}

RaSphere RaShapesFactory::createSphere(RaMaterialAssembly const& material_assembly, float location_x, float location_y, float location_z, float radius) const
{
    return createSphere(material_assembly, float3{ location_x, location_y, location_z }, radius);
}

RaSphere RaShapesFactory::createSphere(float3 const& location, float radius) const
{
    return RaSphere{ m_context, location, radius };
}

RaSphere RaShapesFactory::createSphere(RaMaterialAssembly const& material_assembly, float3 const& location, float radius) const
{
    return RaSphere{ m_context, material_assembly, location, radius };
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
                [this](float location_x, float location_y, float radius)
                {
                    return createCircle(location_x, location_y, radius);
                },
                [this](RaMaterialAssembly const& material_assembly, float location_x, float location_y, float radius)
                {
                    return createCircle(material_assembly, location_x, location_y, radius);
                }
            ),

            "setLocation", 
            lua_support::ListOfOverloads::make_initializer(
                static_cast<void(RaCircle::*)(float2 const&)>(&RaCircle::setLocation),
                static_cast<void(RaCircle::*)(float, float)>(&RaCircle::setLocation)
            ),
            "setRadius", &RaCircle::setRadius,
            "location", &RaCircle::location,
            "radius", &RaCircle::radius);


        lua_support::LuaState::registerSubType<RaSphere>(

            "RaSphere",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{},
                lua_support::BaseClass<RaGeometry>{}
        ),

            lua_support::ListOfFactories::make_initializer(
                [this](float location_x, float location_y, float location_z, float radius)
                {
                    return createSphere(location_x, location_y, location_z, radius);
                },
                [this](RaMaterialAssembly const& material_assembly, 
                    float location_x, float location_y, float location_z, float radius)
                {
                    return createSphere(material_assembly, location_x, location_y, location_z, radius);
                },

                [this](float3 const& location, float radius)
                {
                    return createSphere(location, radius);
                },
                [this](RaMaterialAssembly const& material_assembly, float3 const& location, float radius)
                {
                    return createSphere(material_assembly, location, radius);
                }
            ),

            "setLocation",
            lua_support::ListOfOverloads::make_initializer(
                static_cast<void(RaSphere::*)(float3 const&)>(&RaSphere::setLocation),
                static_cast<void(RaSphere::*)(float, float, float)>(&RaSphere::setLocation)
            ),
            "setRadius", &RaSphere::setRadius,
            "location", &RaSphere::location,
            "radius", &RaSphere::radius);
    }
}
