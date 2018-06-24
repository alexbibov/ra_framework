#include "rendering_pass.h"

using namespace ra;
using namespace ra::rendering_passes;

OxRenderingPass::OxRenderingPass(OxSceneSection const& scene_section):
    m_scene_section{ scene_section }
{
}

OxSceneSection const& OxRenderingPass::targetSceneSection() const
{
    return m_scene_section;
}
