#include "rendering_pass.h"

using namespace ra;
using namespace ra::rendering_passes;

RaRenderingPass::RaRenderingPass(RaSceneSection const& scene_section):
    m_scene_section{ scene_section }
{
}

RaSceneSection const& RaRenderingPass::targetSceneSection() const
{
    return m_scene_section;
}
