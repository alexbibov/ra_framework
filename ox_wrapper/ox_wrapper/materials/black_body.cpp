#include "vector_types.h"

#include "black_body.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ox_wrapper;
using namespace ox_wrapper::materials;

OxBlackBody::OxBlackBody(OxContext const& context, OxRayPayloadType payload_type):
    OxMaterial{ util::Optional<OxProgram>{}, context.createProgram(PTX_BLACK_BODY_MATERIAL, OxProgram::Source::file, OX_SHADER_ENTRY_ANY_HIT) }
{
    static_cast<OxProgram&>(getAnyHitShader()).declareVariable("payload_type", static_cast<unsigned int>(payload_type));
}
