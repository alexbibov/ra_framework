#ifndef OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H
#define OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H

#include "fwd.h"
#include "shapes/fwd.h"
#include "materials/fwd.h"
#include "ray_casters/fwd.h"
#include "rendering_passes/fwd.h"

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

private:
    OxBasicFactory* m_basic_factory;
    shapes::OxShapesFactory* m_shapes_factory;
    materials::OxMaterialsFactory* m_materials_factory;
    ray_casters::OxRaycastersFactory* m_raycasters_factory;
    rendering_passes::OxRenderingPassesFactory* m_rendering_passes_factory;
};

}

#endif
