#include "scene.h"
#include "scene_section.h"

using namespace ox_wrapper;

OxScene::OxScene()
{
}

void OxScene::addSceneSection(OxSceneSection const& scene_section)
{
    if (!scene_section.isValid())
        throw OxException{ ("Scene section \"" + scene_section.getStringName() + "\" is invalid").c_str(),
    __FILE__, __FUNCTION__, __LINE__ };
}

bool OxScene::isValid() const
{
    return m_scene_sections.size() > 0;
}

void OxScene::update() const
{
    for (auto& ss : m_scene_sections)
        ss.update();
}

void OxScene::trace() const
{
    for (auto& ss : m_scene_sections)
        ss.trace();
}
