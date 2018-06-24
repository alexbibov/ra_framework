#include "scene.h"
#include "scene_section.h"
#include "ray_generator.h"

using namespace ra;

OxScene::OxScene()
{
}

void OxScene::addSceneSection(OxSceneSection const& scene_section, OxRayGenerator const* ray_caster)
{
    if (!scene_section.isValid())
        THROW_OX_WRAPPER_ERROR("Scene section \"" + scene_section.getStringName() + "\" is invalid");

    if (!ray_caster->isValid())
        THROW_OX_WRAPPER_ERROR("Ray generator \"" + ray_caster->getStringName() + "\" is invalid");

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
