#include "rendering_pass.h"
#include "scene_section.h"

using namespace ra;
using namespace ra::rendering_passes;

RaRenderingPass::RaRenderingPass(RaSceneSection& scene_section):
    RaContractWithRaContext{ scene_section.context() }
    , m_scene_section{ scene_section }
    , m_rendering_pass_ready{ false }
{
}

void RaRenderingPass::prepare()
{
    prepare_impl();

    m_rendering_pass_ready = true;
}

void RaRenderingPass::render() const
{
    if (!m_rendering_pass_ready)
    {
        THROW_RA_ERROR("Unable to run rendering pass for scene section \""
            + m_scene_section.getStringName() + "\": the rendering pass is not ready")
    }

    render_impl();
}
