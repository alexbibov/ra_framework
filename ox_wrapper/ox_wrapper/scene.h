#ifndef OX_WRAPPER_OPTIX_SCENE_H
#define OX_WRAPPER_OPTIX_SCENE_H

#include "context.h"
#include "fwd.h"
#include <list>

namespace ox_wrapper{

class OxScene final : public OxEntity
{
public:
    OxScene();
    void addSceneSection(OxSceneSection const& scene_section, OxRayGenerator const* ray_generator);

    // required by OxEntity interface
    bool isValid() const override;

    void trace() const;

private:
    std::list<std::pair<OxSceneSection, OxRayGenerator const*>> m_scene_sections;
};

}

#endif