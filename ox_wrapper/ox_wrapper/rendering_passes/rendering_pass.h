#ifndef OX_WRAPPER_RENDERING_PASSES_RENDERING_PASS_H
#define OX_WRAPPER_RENDERING_PASSES_RENDERING_PASS_H

#include "ox_wrapper/ox_wrapper_fwd.h"
#include "ox_wrapper/contract_with_context.h"

namespace ox_wrapper { namespace rendering_passes {

class OxRenderingPass
{
public:
    OxRenderingPass(OxSceneSection const& scene_section);

    OxSceneSection const& targetSceneSection() const;

    virtual void render() const = 0;

private:
    OxSceneSection const& m_scene_section;
};

}}

#endif
