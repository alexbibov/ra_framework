#ifndef OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H
#define OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H

#include "ra/ra_fwd.h"
#include "ra/shapes/ra_shapes_fwd.h"
#include "ra/materials/ra_materials_fwd.h"
#include "ra/ray_casters/ra_ray_casters_fwd.h"
#include "ra/rendering_passes/ra_rendering_passes_fwd.h"
#include "ra/data_store_agents/ra_data_store_agents_fwd.h"
#include "ra/lua_utility_functions_library.h"

namespace ra {

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
    lua_libs::OxLuaLibsMainRegistrar m_lua_libs_main_registrar;
};

}

#endif
