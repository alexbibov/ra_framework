#include "factory_initializer_sentinel.h"

#include "basic_factory.h"
#include "shapes/shapes_factory.h"
#include "materials/materials_factory.h"
#include "ray_casters/ray_casters_factory.h"
#include "rendering_passes/rendering_passes_factory.h"
#include "data_store_agents/data_store_agents_factory.h"

using namespace ra;

RaFactoryInitializerSentinel::RaFactoryInitializerSentinel(RaContext const& context)
{
    m_basic_factory = RaBasicFactory::initialize(context);
    m_shapes_factory = shapes::RaShapesFactory::initialize(context);
    m_materials_factory = materials::RaMaterialsFactory::initialize(context);
    m_raycasters_factory = ray_casters::RaRaycastersFactory::initialize(context);
    m_rendering_passes_factory = rendering_passes::RaRenderingPassesFactory::initialize(context);
    m_data_store_agents_factory = data_store_agents::RaDataStoreAgentsFactory::initialize(context);
}

RaFactoryInitializerSentinel::~RaFactoryInitializerSentinel()
{
    RaBasicFactory::shutdown();
    shapes::RaShapesFactory::shutdown();
    materials::RaMaterialsFactory::shutdown();
    ray_casters::RaRaycastersFactory::shutdown();
    rendering_passes::RaRenderingPassesFactory::shutdown();
    data_store_agents::RaDataStoreAgentsFactory::shutdown();
}

RaBasicFactory const& RaFactoryInitializerSentinel::basicFactory() const
{
    return *m_basic_factory;
}

shapes::RaShapesFactory const& RaFactoryInitializerSentinel::shapesFactory() const
{
    return *m_shapes_factory;
}

materials::RaMaterialsFactory const& RaFactoryInitializerSentinel::materialsFactory() const
{
    return *m_materials_factory;
}

ray_casters::RaRaycastersFactory const& RaFactoryInitializerSentinel::raycastersFactory() const
{
    return *m_raycasters_factory;
}

rendering_passes::RaRenderingPassesFactory const& RaFactoryInitializerSentinel::renderingPassesFactory() const
{
    return *m_rendering_passes_factory;
}

data_store_agents::RaDataStoreAgentsFactory const& RaFactoryInitializerSentinel::dataStoreAgentsFactory() const
{
    return *m_data_store_agents_factory;
}
