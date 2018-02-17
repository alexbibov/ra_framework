#include "vector_types.h"

#include "black_body.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ox_wrapper;
using namespace ox_wrapper::materials;

OxBlackBody::OxBlackBody(OxContext const& context, OxRayPayloadType payload_type, OxRayType affected_ray_type):
    OxMaterial{ util::Optional<OxProgram>{}, context.createProgram(PTX_BLACK_BODY_MATERIAL, OxProgram::Source::file, OX_SHADER_ENTRY_ANY_HIT), affected_ray_type }
{
    static_cast<OxProgram&>(getAnyHitShader()).setVariableValue("payload_type", static_cast<unsigned int>(payload_type));
}
