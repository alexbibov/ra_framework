#include "scene.h"
#include "scene_section.h"
#include "ray_generator.h"

using namespace ox_wrapper;

OxScene::OxScene()
{
}

void OxScene::addSceneSection(OxSceneSection const& scene_section, OxRayGenerator const* ray_caster)
{
    if (!scene_section.isValid())
        throw OxException{ ("Scene section \"" + scene_section.getStringName() + "\" is invalid").c_str(),
    __FILE__, __FUNCTION__, __LINE__ };

    if (!ray_caster->isValid())
        throw OxException{ ("Ray generator \"" + ray_caster->getStringName() + "\" is invalid").c_str(),
    __FILE__, __FUNCTION__, __LINE__ };

    m_scene_sections.push_back(std::make_pair(scene_section, ray_caster));
}

bool OxScene::isValid() const
{
    return m_scene_sections.size() > 0;
}

void OxScene::trace() const
{
    for (auto& ss : m_scene_sections)
        ss.first.trace(*ss.second);
}
