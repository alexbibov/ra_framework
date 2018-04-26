#include "recaster_generator.h"

#include "../context.h"
#include "../../ptx.h"

using namespace ox_wrapper;
using namespace ox_wrapper::ray_casters;

namespace
{

std::string create_output_buffer_binding_name(OxAbstractBuffer const& output_buffer, 
    OxRayPayloadType payload_type)
{
    std::string rv{};

    switch (payload_type)
    {
    case OxRayPayloadType::radiance:
        rv += "ox_radiance_payload_";
        break;

    case OxRayPayloadType::radiance_simple:
        rv += "ox_radiance_payload_simple_";
        break;

    case OxRayPayloadType::monochromatic:
        rv += "ox_radiance_payload_monochromatic_";
        break;

    case OxRayPayloadType::occlusion:
        rv += "ox_occlusion_payload_";
        break;

    default:
        throw OxException{ "unknown payload type", __FILE__, __FUNCTION__, __LINE__ };
    }

    switch (output_buffer.getDimension())
    {
    case OxBufferDimension::_1D:
        rv += "1d_out";
        break;

    case OxBufferDimension::_2D:
        rv += "2d_out";
        break;

    case OxBufferDimension::_3D:
        rv += "3d_out";
        break;

    default:
        throw OxException{ "unknown buffer dimension", __FILE__, __FUNCTION__, __LINE__ };
    }

    return rv;
}

}


class OxRecasterGenerator::impl
{
public:
    impl(OxRecasterGenerator const& parent, OxBufferDimension target_dimension, OxRayPayloadType target_payload_type):
        m_ray_radiance_payload_1d_dummy{ parent.context().createBuffer<OxRayRadiancePayload>(OxBufferKind::input_output, 1) },
        m_ray_radiance_payload_2d_dummy{ parent.context().createBuffer<OxRayRadiancePayload>(OxBufferKind::input_output, 1, 1) },
        m_ray_radiance_payload_3d_dummy{ parent.context().createBuffer<OxRayRadiancePayload>(OxBufferKind::input_output, 1, 1, 1) },

        m_ray_radiance_payload_simple_1d_dummy{ parent.context().createBuffer<OxRayRadiancePayloadSimple>(OxBufferKind::input_output, 1) },
        m_ray_radiance_payload_simple_2d_dummy{ parent.context().createBuffer<OxRayRadiancePayloadSimple>(OxBufferKind::input_output, 1, 1) },
        m_ray_radiance_payload_simple_3d_dummy{ parent.context().createBuffer<OxRayRadiancePayloadSimple>(OxBufferKind::input_output, 1, 1, 1) },

        m_ray_radiance_payload_monochromatic_1d_dummy{ parent.context().createBuffer<OxRayRadiancePayloadMonochromatic>(OxBufferKind::input_output, 1) },
        m_ray_radiance_payload_monochromatic_2d_dummy{ parent.context().createBuffer<OxRayRadiancePayloadMonochromatic>(OxBufferKind::input_output, 1, 1) },
        m_ray_radiance_payload_monochromatic_3d_dummy{ parent.context().createBuffer<OxRayRadiancePayloadMonochromatic>(OxBufferKind::input_output, 1, 1, 1) },

        m_ray_occlusion_payload_1d_dummy{ parent.context().createBuffer<OxRayOcclusionPayload>(OxBufferKind::input_output, 1) },
        m_ray_occlusion_payload_2d_dummy{ parent.context().createBuffer<OxRayOcclusionPayload>(OxBufferKind::input_output, 1, 1) },
        m_ray_occlusion_payload_3d_dummy{ parent.context().createBuffer<OxRayOcclusionPayload>(OxBufferKind::input_output, 1, 1, 1) }
    {
        parent.getRayGenerationShader().setVariableValue("dimension", static_cast<unsigned int>(target_dimension));
        parent.getRayGenerationShader().setVariableValue("payload_type", static_cast<unsigned int>(target_payload_type));

        switch (target_payload_type)
        {
        case OxRayPayloadType::radiance:
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_1d_out", m_ray_radiance_payload_simple_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_2d_out", m_ray_radiance_payload_simple_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_3d_out", m_ray_radiance_payload_simple_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_monochromatic_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_monochromatic_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_monochromatic_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_1d_out", m_ray_occlusion_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_2d_out", m_ray_occlusion_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_3d_out", m_ray_occlusion_payload_3d_dummy);

            switch (target_dimension)
            {
            case OxBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;


        case OxRayPayloadType::radiance_simple:
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_monochromatic_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_monochromatic_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_monochromatic_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_1d_out", m_ray_occlusion_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_2d_out", m_ray_occlusion_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_3d_out", m_ray_occlusion_payload_3d_dummy);

            switch (target_dimension)
            {
            case OxBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;


        case OxRayPayloadType::monochromatic:
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_1d_out", m_ray_radiance_payload_simple_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_2d_out", m_ray_radiance_payload_simple_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_3d_out", m_ray_radiance_payload_simple_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_1d_out", m_ray_occlusion_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_2d_out", m_ray_occlusion_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_3d_out", m_ray_occlusion_payload_3d_dummy);

            switch (target_dimension)
            {
            case OxBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;


        case OxRayPayloadType::occlusion:
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_1d_out", m_ray_radiance_payload_simple_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_2d_out", m_ray_radiance_payload_simple_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_3d_out", m_ray_radiance_payload_simple_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_monochromatic_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_monochromatic_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_monochromatic_3d_dummy);

            switch (target_dimension)
            {
            case OxBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case OxBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ox_occlusion_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;
        }
    }

private:
    OxBuffer<OxRayRadiancePayload> m_ray_radiance_payload_1d_dummy;
    OxBuffer<OxRayRadiancePayload> m_ray_radiance_payload_2d_dummy;
    OxBuffer<OxRayRadiancePayload> m_ray_radiance_payload_3d_dummy;

    OxBuffer<OxRayRadiancePayloadSimple> m_ray_radiance_payload_simple_1d_dummy;
    OxBuffer<OxRayRadiancePayloadSimple> m_ray_radiance_payload_simple_2d_dummy;
    OxBuffer<OxRayRadiancePayloadSimple> m_ray_radiance_payload_simple_3d_dummy;

    OxBuffer<OxRayRadiancePayloadMonochromatic> m_ray_radiance_payload_monochromatic_1d_dummy;
    OxBuffer<OxRayRadiancePayloadMonochromatic> m_ray_radiance_payload_monochromatic_2d_dummy;
    OxBuffer<OxRayRadiancePayloadMonochromatic> m_ray_radiance_payload_monochromatic_3d_dummy;

    OxBuffer<OxRayOcclusionPayload> m_ray_occlusion_payload_1d_dummy;
    OxBuffer<OxRayOcclusionPayload> m_ray_occlusion_payload_2d_dummy;
    OxBuffer<OxRayOcclusionPayload> m_ray_occlusion_payload_3d_dummy;
};


OxRecasterGenerator::OxRecasterGenerator(
    uint3 const& original_problem_size,
    OxTraverseBackupBuffer const& traverse_backup_buffer, 
    OxBuffer<OxRayRadiancePayload> const& output_buffer, 
    OxRayType recasted_ray_type, float recasted_ray_parametric_length):
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, OxRayPayloadType::radiance), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), OxBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{*this, output_buffer.getDimension(), OxRayPayloadType::radiance} }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

OxRecasterGenerator::OxRecasterGenerator(
    uint3 const& original_problem_size,
    OxTraverseBackupBuffer const& traverse_backup_buffer, 
    OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer, 
    OxRayType recasted_ray_type, float recasted_ray_parametric_length) :
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, OxRayPayloadType::radiance_simple), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), OxBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{ *this, output_buffer.getDimension(), OxRayPayloadType::radiance_simple } }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

OxRecasterGenerator::OxRecasterGenerator(
    uint3 const& original_problem_size,
    OxTraverseBackupBuffer const& traverse_backup_buffer, 
    OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer, 
    OxRayType recasted_ray_type, float recasted_ray_parametric_length) :
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, OxRayPayloadType::monochromatic), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), OxBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{ *this, output_buffer.getDimension(), OxRayPayloadType::monochromatic } }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

OxRecasterGenerator::OxRecasterGenerator(
    uint3 const& original_problem_size,
    OxTraverseBackupBuffer const& traverse_backup_buffer, 
    OxBuffer<OxRayOcclusionPayload> const& output_buffer, 
    OxRayType recasted_ray_type, float recasted_ray_parametric_length) :
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, OxRayPayloadType::occlusion), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), OxBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{ *this, output_buffer.getDimension(), OxRayPayloadType::occlusion } }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

void OxRecasterGenerator::update(OxObjectHandle top_scene_object) const
{
    getRayGenerationShader().assignBuffer("traverse_backup_buffer", m_traverse_backup_buffer.readBuffer());

    auto mapping = makeBufferMapSentry(m_traverse_backup_buffer.readBuffer(), OxBufferMapKind::read_write, 0);
    update_topology(static_cast<uint32_t>(mapping.address()[0]), 1U, 1U);


    OxRayGeneratorWithOutputBuffer::update(top_scene_object);
}

OxRecasterGenerator::~OxRecasterGenerator() = default;
