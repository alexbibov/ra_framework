#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ray_payloads.h"
#include "constants.h"

using namespace optix;
using namespace ra;

rtDeclareVariable(unsigned int, payload_type, , "Type of ray payload");
rtDeclareVariable(RaRayRadiancePayload, radiance_payload, rtPayload, );
rtDeclareVariable(RaRayRadiancePayloadSimple, radiance_payload_simple, rtPayload, );
rtDeclareVariable(RaRayRadiancePayloadMonochromatic, radiance_payload_monochromatic, rtPayload, );
rtDeclareVariable(RaRayOcclusionPayload, occlusion_payload, rtPayload, );
rtDeclareVariable(float, intersection_distance, rtIntersectionDistance, "Parametric distance from ray origin to the intersection");

RT_PROGRAM void __ra_any_hit__()
{
    switch (static_cast<RaRayPayloadType>(payload_type))
    {
    case RaRayPayloadType::radiance:
        memset(radiance_payload.spectral_radiance, 0, constants::max_spectra_pairs_supported * sizeof(float2));

        if (!radiance_payload.depth.x) radiance_payload.depth.x = intersection_distance;
        radiance_payload.depth.y += intersection_distance;
        break;

    case RaRayPayloadType::radiance_simple:
        radiance_payload_simple.spectral_radiance = make_float2(0.f, 0.f);

        if (!radiance_payload.depth.x) radiance_payload.depth.x = intersection_distance;
        radiance_payload.depth.y += intersection_distance;
        break;

    case RaRayPayloadType::monochromatic:
        radiance_payload_monochromatic.spectral_radiance = 0.f;

        if (!radiance_payload.depth.x) radiance_payload.depth.x = intersection_distance;
        radiance_payload.depth.y += intersection_distance;
        break;

    case RaRayPayloadType::occlusion:
        occlusion_payload.is_occluded = true;

        if (!radiance_payload.depth.x) radiance_payload.depth.x = intersection_distance;
        radiance_payload.depth.y += intersection_distance;
        break;
    }

    rtTerminateRay();
}