#ifndef RA_FACTORY_INITIALIZER_SENTINEL_H
#define RA_FACTORY_INITIALIZER_SENTINEL_H

#include "ra/ra_fwd.h"
#include "ra/shapes/ra_shapes_fwd.h"
#include "ra/materials/ra_materials_fwd.h"
#include "ra/ray_casters/ra_ray_casters_fwd.h"
#include "ra/rendering_passes/ra_rendering_passes_fwd.h"
#include "ra/data_store_agents/ra_data_store_agents_fwd.h"
#include "ra/lua_utility_functions_library.h"

namespace ra {

class RaFactoryInitializerSentinel final
{
public:
    RaFactoryInitializerSentinel(RaContext const& context);
    ~RaFactoryInitializerSentinel();

    RaFactoryInitializerSentinel(RaFactoryInitializerSentinel const&) = delete;
    RaFactoryInitializerSentinel(RaFactoryInitializerSentinel&&) = delete;

    RaFactoryInitializerSentinel& operator=(RaFactoryInitializerSentinel const&) = delete;
    RaFactoryInitializerSentinel& operator=(RaFactoryInitializerSentinel&&) = delete;

    RaBasicFactory const& basicFactory() const;
    shapes::RaShapesFactory const& shapesFactory() const;
    materials::RaMaterialsFactory const& materialsFactory() const;
    ray_casters::RaRaycastersFactory const& raycastersFactory() const;
    rendering_passes::RaRenderingPassesFactory const& renderingPassesFactory() const;
    data_store_agents::RaDataStoreAgentsFactory const& dataStoreAgentsFactory() const;

private:
    RaBasicFactory* m_basic_factory;
    shapes::RaShapesFactory* m_shapes_factory;
    materials::RaMaterialsFactory* m_materials_factory;
    ray_casters::RaRaycastersFactory* m_raycasters_factory;
    rendering_passes::RaRenderingPassesFactory* m_rendering_passes_factory;
    data_store_agents::RaDataStoreAgentsFactory* m_data_store_agents_factory;
    lua_libs::RaLuaLibsMainRegistrar m_lua_libs_main_registrar;
};

}

#endif
