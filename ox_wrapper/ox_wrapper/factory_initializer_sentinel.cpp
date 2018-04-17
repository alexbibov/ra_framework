#include "factory_initializer_sentinel.h"

#include "basic_factory.h"
#include "shapes/shapes_factory.h"
#include "materials/materials_factory.h"
#include "ray_casters/ray_casters_factory.h"
#include "rendering_passes/rendering_passes_factory.h"
#include "data_store_agents/data_store_agents_factory.h"

using namespace ox_wrapper;

OxFactoryInitializerSentinel::OxFactoryInitializerSentinel(OxContext const& context)
{
    m_basic_factory = OxBasicFactory::initialize(context);
    m_shapes_factory = shapes::OxShapesFactory::initialize(context);
    m_materials_factory = materials::OxMaterialsFactory::initialize(context);
    m_raycasters_factory = ray_casters::OxRaycastersFactory::initialize(context);
    m_rendering_passes_factory = rendering_passes::OxRenderingPassesFactory::initialize(context);
    m_data_store_agents_factory = data_store_agents::OxDataStoreAgentsFactory::initialize(context);
}

OxFactoryInitializerSentinel::~OxFactoryInitializerSentinel()
{
    OxBasicFactory::shutdown();
    shapes::OxShapesFactory::shutdown();
    materials::OxMaterialsFactory::shutdown();
    ray_casters::OxRaycastersFactory::shutdown();
    rendering_passes::OxRenderingPassesFactory::shutdown();
    data_store_agents::OxDataStoreAgentsFactory::shutdown();
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

rendering_passes::OxRenderingPassesFactory const& OxFactoryInitializerSentinel::renderingPassesFactory() const
{
    return *m_rendering_passes_factory;
}

data_store_agents::OxDataStoreAgentsFactory const& OxFactoryInitializerSentinel::dataStoreAgentsFactory() const
{
    return *m_data_store_agents_factory;
}
