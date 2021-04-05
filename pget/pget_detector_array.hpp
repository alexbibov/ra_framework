#ifndef RA_PGET_DETECTOR_ARRAY_HPP
#define RA_PGET_DETECTOR_ARRAY_HPP

#include <vector>
#include <vector_types.h>
#include <ray_generator_with_output_buffer.h>

namespace ra::pget {


struct DetectorChamberDesc
{
    float2 detector_dimensions;    //!< physical width and height of the detector represented in mm
    float2 detector_side_collimator_opening;    //!< width and height of the collimator opening on the detector side represented in mm
    float2 fuel_side_collimator_opening;    //!< width and height of the collimator opening on the fuel side represented in mm
    float collimator_depth;    //!< depth of the collimator in mm
    float detector_to_collimator_distance;    //!< distance between the detector on the collimator in mm
};


enum class DetectorArrayRayTracingMode
{
    _2D,
    _3D
};


class DetectorArray : public RaRayGeneratorWithOutputBuffer
{
public:
    DetectorArray(RaContext const& context, uint32_t detector_count, float detector_spacing,
        DetectorChamberDesc const& single_detection_chamber_desc, float sampling_rate, DetectorArrayRayTracingMode ray_tracing_mode);

private:
    uint32_t m_detector_count;    //!< total number of detectors in the assembly
    float m_detector_spacing;    //!< distance between the central points of two adjacent detectors given in mm
    DetectorChamberDesc m_chamber_descriptor;    //!< descriptor containing physical parameters of a single detection chamber
    DetectorArrayRayTracingMode m_ray_tracing_mode;    //!< controls, whether the ray tracing is perform in 2D or 3D space
};

}

#endif