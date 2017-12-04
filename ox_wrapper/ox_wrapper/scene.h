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
    void addSceneSection(OxSceneSection const& scene_section);

    // required by OxEntity interface
    bool isValid() const override;

    void trace();

private:
    std::list<OxSceneSection> m_scene_sections;
};

}

#endif