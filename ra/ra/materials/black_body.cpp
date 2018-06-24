#include "vector_types.h"

#include "black_body.h"
#include "../../ptx.h"
#include "../context.h"

using namespace ra;
using namespace ra::materials;

RaBlackBody::RaBlackBody(RaContext const& context, RaRayPayloadType payload_type, RaRayTypeCollection affected_ray_types)
    : RaMaterial{ 
        util::Optional<RaProgram>{}, 
        context.createProgram(PTX_BLACK_BODY_MATERIAL, RaProgram::Source::file, OX_SHADER_ENTRY_ANY_HIT), 
        affected_ray_types }
{
    static_cast<RaProgram&>(getAnyHitShader()).setVariableValue("payload_type", static_cast<unsigned int>(payload_type));
}
