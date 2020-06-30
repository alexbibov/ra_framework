#ifndef RA_RENDERING_PASSES_RENDERING_PASS_H
#define RA_RENDERING_PASSES_RENDERING_PASS_H

#include "ra_fwd.h"
#include "contract_with_context.h"

namespace ra { namespace rendering_passes {

class RaRenderingPass: public RaContractWithRaContext
{
public:
    RaRenderingPass(RaSceneSection& scene_section);

    RaSceneSection& targetSceneSection() { return m_scene_section; }
    RaSceneSection const& targetSceneSection() const { return m_scene_section; }

    void prepare();
    void render() const;

protected:
    virtual void prepare_impl() = 0;
    virtual void render_impl() const = 0;

private:
    RaSceneSection& m_scene_section;
    bool m_rendering_pass_ready;
};

}}

#endif
