#ifndef OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H
#define OX_WRAPPER_FACTORY_INITIALIZER_SENTINEL_H

#include "fwd.h"
#include "shapes/fwd.h"

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

private:
    OxBasicFactory* m_basic_factory;
    shapes::OxShapesFactory* m_shapes_factory;
};

}

#endif
