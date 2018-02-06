#include "factory_initializer_sentinel.h"

#include "basic_factory.h"
#include "shapes/shapes_factory.h"
#include "materials/materials_factory.h"
#include "ray_casters/ray_casters_factory.h"

using namespace ox_wrapper;

OxFactoryInitializerSentinel::OxFactoryInitializerSentinel(OxContext const& context)
{
    m_basic_factory = OxBasicFactory::initialize(context);
    m_shapes_factory = shapes::OxShapesFactory::initialize(context);
    m_materials_factory = materials::OxMaterialsFactory::initialize(context);
    m_raycasters_factory = ray_casters::OxRaycastersFactory::initialize(context);
}

OxFactoryInitializerSentinel::~OxFactoryInitializerSentinel()
{
    OxBasicFactory::shutdown();
    shapes::OxShapesFactory::shutdown();
    materials::OxMaterialsFactory::shutdown();
    ray_casters::OxRaycastersFactory::shutdown();
}

OxBasicFactory const& OxFactoryInitializerSentinel::basicFactory() const
{
    return *m_basic_factory;
}

shapes::OxShapesFactory const& OxFactoryInitializerSentinel::shapesFactory() const
{
    return *m_shapes_factory;
}

materials::OxMaterialsFactory const& OxFactoryInitializerSentinel::materialsFactory() const
{
    return *m_materials_factory;
}

ray_casters::OxRaycastersFactory const& OxFactoryInitializerSentinel::raycastersFactory() const
{
    return *m_raycasters_factory;
}