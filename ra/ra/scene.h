#ifndef RA_OPTIX_SCENE_H
#define RA_OPTIX_SCENE_H

#include "ra/context.h"
#include "ra/ra_fwd.h"
#include <list>

namespace ra{

class RaScene final : public RaEntity
{
public:
    RaScene();
    void addSceneSection(RaSceneSection const& scene_section, RaRayGenerator const* ray_generator);

    // required by RaEntity interface
    bool isValid() const override;

    void trace() const;

private:
    std::list<std::pair<RaSceneSection, RaRayGenerator const*>> m_scene_sections;
};

}

#endif