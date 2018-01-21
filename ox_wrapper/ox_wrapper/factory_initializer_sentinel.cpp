#include "factory_initializer_sentinel.h"

#include "basic_factory.h"
#include "shapes/shapes_factory.h"

using namespace ox_wrapper;

OxFactoryInitializerSentinel::OxFactoryInitializerSentinel(OxContext const& context)
{
    m_basic_factory = OxBasicFactory::initialize(context);
    m_shapes_factory = shapes::OxShapesFactory::initialize(context);
}

OxFactoryInitializerSentinel::~OxFactoryInitializerSentinel()
{
    OxBasicFactory::shutdown();
    shapes::OxShapesFactory::shutdown();
}

OxBasicFactory const& OxFactoryInitializerSentinel::basicFactory() const
{
    return *m_basic_factory;
}

shapes::OxShapesFactory const& OxFactoryInitializerSentinel::shapesFactory() const
{
    return *m_shapes_factory;
}
