#ifndef OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H
#define OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H

#include "ox_wrapper/ox_wrapper_fwd.h"
#include "ox_wrapper/shapes/ox_wrapper_shapes_fwd.h"
#include "ox_wrapper/materials/ox_wrapper_materials_fwd.h"
#include "ox_wrapper/ray_casters/ox_wrapper_ray_casters_fwd.h"
#include "ox_wrapper/rendering_passes/ox_wrapper_rendering_passes_fwd.h"
#include "ox_wrapper/data_store_agents/ox_wrapper_data_store_agents_fwd.h"

namespace ox_wrapper {

class OxFactoryInitializerSentinel final
{
public:
    OxFactoryInitializerSentinel(OxContext const& context);
    ~OxFactoryInitializerSentinel();

    OxFactoryInitializerSentinel(OxFactoryInitializerSentinel const&) = delete;
    OxFactoryInitializerSentinel(OxFactoryInitializerSentinel&&) = delete;

    OxFactoryInitializerSentinel& operator=(OxFactoryInitializerSentinel const&) = delete;
    OxFactoryInitializerSentinel& operator=(OxFactoryInitializerSentinel&&) = delete;

    OxBasicFactory const& basicFactory() const;
    shapes::OxShapesFactory const& shapesFactory() const;
    materials::OxMaterialsFactory const& materialsFactory() const;
    ray_casters::OxRaycastersFactory const& raycastersFactory() const;
    rendering_passes::OxRenderingPassesFactory const& renderingPassesFactory() const;
    data_store_agents::OxDataStoreAgentsFactory const& dataStoreAgentsFactory() const;

private:
    OxBasicFactory* m_basic_factory;
    shapes::OxShapesFactory* m_shapes_factory;
    materials::OxMaterialsFactory* m_materials_factory;
    ray_casters::OxRaycastersFactory* m_raycasters_factory;
    rendering_passes::OxRenderingPassesFactory* m_rendering_passes_factory;
    data_store_agents::OxDataStoreAgentsFactory* m_data_store_agents_factory;
};

}

#endif
