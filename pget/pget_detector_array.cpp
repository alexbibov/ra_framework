#include <cmath>

#include "context.h"

#include "pget_detector_array.hpp"
#include "payload.h"
#include "ptx.h"


using namespace ra;
using namespace ra::pget;


namespace {

unsigned int getRayXResolution(DetectorChamberDesc const& detector_chamber_desc, float sampling_rate)
{
    return static_cast<unsigned int>(std::ceilf(sampling_rate * detector_chamber_desc.detector_dimensions.y));
}

unsigned int getRayYResolution(DetectorChamberDesc const& detector_chamber_desc, float sampling_rate)
{
    return static_cast<unsigned int>(std::ceilf(sampling_rate * detector_chamber_desc.detector_dimensions.x));
}

}

DetectorArray::DetectorArray(RaContext const& context, uint32_t detector_count, float detector_spacing,
    DetectorChamberDesc const& single_detection_chamber_desc, float sampling_rate, DetectorArrayRayTracingMode ray_tracing_mode)
    : RaRayGeneratorWithOutputBuffer{
    context.createProgram(PTX_PGET_DEVICE, RaProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    context.createBuffer<Payload>(RaBufferKind::input, getGeneratorDimensions().x, getGeneratorDimensions().y),
    "output_buffer",
    getRayXResolution(single_detection_chamber_desc, sampling_rate) * detector_count,
    getRayYResolution(single_detection_chamber_desc, sampling_rate) }

    , m_detector_count{ detector_count }
    , m_detector_spacing{ detector_spacing }
    , m_chamber_descriptor{ single_detection_chamber_desc }
    , m_ray_tracing_mode{ ray_tracing_mode }
{

}