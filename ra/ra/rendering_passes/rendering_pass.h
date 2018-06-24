#ifndef OX_WRAPPER_RENDERING_PASSES_RENDERING_PASS_H
#define OX_WRAPPER_RENDERING_PASSES_RENDERING_PASS_H

#include "ra/ra_fwd.h"
#include "ra/contract_with_context.h"

namespace ra { namespace rendering_passes {

class RaRenderingPass
{
public:
    RaRenderingPass(RaSceneSection const& scene_section);

    RaSceneSection const& targetSceneSection() const;

    virtual void render() const = 0;

private:
    RaSceneSection const& m_scene_section;
};

}}

#endif
