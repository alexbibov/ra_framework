#include "recaster_generator.h"

#include "context.h"
#include "ptx.h"

using namespace ra;
using namespace ra::ray_casters;

namespace
{

std::string create_output_buffer_binding_name(RaAbstractBuffer const& output_buffer, 
    RaRayPayloadType payload_type)
{
    std::string rv{};

    switch (payload_type)
    {
    case RaRayPayloadType::radiance:
        rv += "ra_radiance_payload_";
        break;

    case RaRayPayloadType::radiance_simple:
        rv += "ra_radiance_payload_simple_";
        break;

    case RaRayPayloadType::monochromatic:
        rv += "ra_radiance_payload_monochromatic_";
        break;

    case RaRayPayloadType::occlusion:
        rv += "ra_occlusion_payload_";
        break;

    default:
        THROW_RA_ERROR("unknown payload type");
    }

    switch (output_buffer.getDimension())
    {
    case RaBufferDimension::_1D:
        rv += "1d_out";
        break;

    case RaBufferDimension::_2D:
        rv += "2d_out";
        break;

    case RaBufferDimension::_3D:
        rv += "3d_out";
        break;

    default:
        THROW_RA_ERROR("unknown buffer dimension");
    }

    return rv;
}

}


class RaRecasterGenerator::impl
{
public:
    impl(RaRecasterGenerator const& parent, RaBufferDimension target_dimension, RaRayPayloadType target_payload_type):
        m_ray_radiance_payload_1d_dummy{ parent.context().createBuffer<RaRayRadiancePayload>(RaBufferKind::input_output, 1) },
        m_ray_radiance_payload_2d_dummy{ parent.context().createBuffer<RaRayRadiancePayload>(RaBufferKind::input_output, 1, 1) },
        m_ray_radiance_payload_3d_dummy{ parent.context().createBuffer<RaRayRadiancePayload>(RaBufferKind::input_output, 1, 1, 1) },

        m_ray_radiance_payload_simple_1d_dummy{ parent.context().createBuffer<RaRayRadiancePayloadSimple>(RaBufferKind::input_output, 1) },
        m_ray_radiance_payload_simple_2d_dummy{ parent.context().createBuffer<RaRayRadiancePayloadSimple>(RaBufferKind::input_output, 1, 1) },
        m_ray_radiance_payload_simple_3d_dummy{ parent.context().createBuffer<RaRayRadiancePayloadSimple>(RaBufferKind::input_output, 1, 1, 1) },

        m_ray_radiance_payload_monochromatic_1d_dummy{ parent.context().createBuffer<RaRayRadiancePayloadMonochromatic>(RaBufferKind::input_output, 1) },
        m_ray_radiance_payload_monochromatic_2d_dummy{ parent.context().createBuffer<RaRayRadiancePayloadMonochromatic>(RaBufferKind::input_output, 1, 1) },
        m_ray_radiance_payload_monochromatic_3d_dummy{ parent.context().createBuffer<RaRayRadiancePayloadMonochromatic>(RaBufferKind::input_output, 1, 1, 1) },

        m_ray_occlusion_payload_1d_dummy{ parent.context().createBuffer<RaRayOcclusionPayload>(RaBufferKind::input_output, 1) },
        m_ray_occlusion_payload_2d_dummy{ parent.context().createBuffer<RaRayOcclusionPayload>(RaBufferKind::input_output, 1, 1) },
        m_ray_occlusion_payload_3d_dummy{ parent.context().createBuffer<RaRayOcclusionPayload>(RaBufferKind::input_output, 1, 1, 1) }
    {
        parent.getRayGenerationShader().setVariableValue("dimension", static_cast<unsigned int>(target_dimension));
        parent.getRayGenerationShader().setVariableValue("payload_type", static_cast<unsigned int>(target_payload_type));

        switch (target_payload_type)
        {
        case RaRayPayloadType::radiance:
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_1d_out", m_ray_radiance_payload_simple_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_2d_out", m_ray_radiance_payload_simple_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_3d_out", m_ray_radiance_payload_simple_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_monochromatic_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_monochromatic_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_monochromatic_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_1d_out", m_ray_occlusion_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_2d_out", m_ray_occlusion_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_3d_out", m_ray_occlusion_payload_3d_dummy);

            switch (target_dimension)
            {
            case RaBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;


        case RaRayPayloadType::radiance_simple:
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_monochromatic_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_monochromatic_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_monochromatic_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_1d_out", m_ray_occlusion_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_2d_out", m_ray_occlusion_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_3d_out", m_ray_occlusion_payload_3d_dummy);

            switch (target_dimension)
            {
            case RaBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;


        case RaRayPayloadType::monochromatic:
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_1d_out", m_ray_radiance_payload_simple_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_2d_out", m_ray_radiance_payload_simple_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_3d_out", m_ray_radiance_payload_simple_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_1d_out", m_ray_occlusion_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_2d_out", m_ray_occlusion_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_3d_out", m_ray_occlusion_payload_3d_dummy);

            switch (target_dimension)
            {
            case RaBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;


        case RaRayPayloadType::occlusion:
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_1d_out", m_ray_radiance_payload_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_2d_out", m_ray_radiance_payload_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_3d_out", m_ray_radiance_payload_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_1d_out", m_ray_radiance_payload_simple_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_2d_out", m_ray_radiance_payload_simple_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_simple_3d_out", m_ray_radiance_payload_simple_3d_dummy);

            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_1d_out", m_ray_radiance_payload_monochromatic_1d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_2d_out", m_ray_radiance_payload_monochromatic_2d_dummy);
            parent.getRayGenerationShader().assignBuffer("ra_radiance_payload_monochromatic_3d_out", m_ray_radiance_payload_monochromatic_3d_dummy);

            switch (target_dimension)
            {
            case RaBufferDimension::_1D:
                parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_2D:
                parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_3d_out", m_ray_radiance_payload_3d_dummy);
                break;
            case RaBufferDimension::_3D:
                parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_1d_out", m_ray_radiance_payload_1d_dummy);
                parent.getRayGenerationShader().assignBuffer("ra_occlusion_payload_2d_out", m_ray_radiance_payload_2d_dummy);
                break;
            }

            break;
        }
    }

private:
    RaBuffer<RaRayRadiancePayload> m_ray_radiance_payload_1d_dummy;
    RaBuffer<RaRayRadiancePayload> m_ray_radiance_payload_2d_dummy;
    RaBuffer<RaRayRadiancePayload> m_ray_radiance_payload_3d_dummy;

    RaBuffer<RaRayRadiancePayloadSimple> m_ray_radiance_payload_simple_1d_dummy;
    RaBuffer<RaRayRadiancePayloadSimple> m_ray_radiance_payload_simple_2d_dummy;
    RaBuffer<RaRayRadiancePayloadSimple> m_ray_radiance_payload_simple_3d_dummy;

    RaBuffer<RaRayRadiancePayloadMonochromatic> m_ray_radiance_payload_monochromatic_1d_dummy;
    RaBuffer<RaRayRadiancePayloadMonochromatic> m_ray_radiance_payload_monochromatic_2d_dummy;
    RaBuffer<RaRayRadiancePayloadMonochromatic> m_ray_radiance_payload_monochromatic_3d_dummy;

    RaBuffer<RaRayOcclusionPayload> m_ray_occlusion_payload_1d_dummy;
    RaBuffer<RaRayOcclusionPayload> m_ray_occlusion_payload_2d_dummy;
    RaBuffer<RaRayOcclusionPayload> m_ray_occlusion_payload_3d_dummy;
};


RaRecasterGenerator::RaRecasterGenerator(
    uint3 const& original_problem_size,
    RaTraverseBackupBuffer const& traverse_backup_buffer, 
    RaBuffer<RaRayRadiancePayload> const& output_buffer, 
    RaRayType recasted_ray_type, float recasted_ray_parametric_length):
    RaRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, RaProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, RaRayPayloadType::radiance), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), RaBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{*this, output_buffer.getDimension(), RaRayPayloadType::radiance} }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

RaRecasterGenerator::RaRecasterGenerator(
    uint3 const& original_problem_size,
    RaTraverseBackupBuffer const& traverse_backup_buffer, 
    RaBuffer<RaRayRadiancePayloadSimple> const& output_buffer, 
    RaRayType recasted_ray_type, float recasted_ray_parametric_length) :
    RaRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, RaProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, RaRayPayloadType::radiance_simple), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), RaBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{ *this, output_buffer.getDimension(), RaRayPayloadType::radiance_simple } }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

RaRecasterGenerator::RaRecasterGenerator(
    uint3 const& original_problem_size,
    RaTraverseBackupBuffer const& traverse_backup_buffer, 
    RaBuffer<RaRayRadiancePayloadMonochromatic> const& output_buffer, 
    RaRayType recasted_ray_type, float recasted_ray_parametric_length) :
    RaRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, RaProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, RaRayPayloadType::monochromatic), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), RaBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{ *this, output_buffer.getDimension(), RaRayPayloadType::monochromatic } }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

RaRecasterGenerator::RaRecasterGenerator(
    uint3 const& original_problem_size,
    RaTraverseBackupBuffer const& traverse_backup_buffer, 
    RaBuffer<RaRayOcclusionPayload> const& output_buffer, 
    RaRayType recasted_ray_type, float recasted_ray_parametric_length) :
    RaRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, RaProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer, RaRayPayloadType::occlusion), 
    makeBufferMapSentry(traverse_backup_buffer.readBuffer(), RaBufferMapKind::read).address()[0] },
    m_traverse_backup_buffer{ traverse_backup_buffer },
    m_impl{ new impl{ *this, output_buffer.getDimension(), RaRayPayloadType::occlusion } }
{
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().setVariableValue("ray_parametric_length", recasted_ray_parametric_length);
    getRayGenerationShader().setVariableValue("problem_size", original_problem_size);
    // getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

void RaRecasterGenerator::update(RaObjectHandle top_scene_object) const
{
    getRayGenerationShader().assignBuffer("traverse_backup_buffer", m_traverse_backup_buffer.readBuffer());

    auto mapping = makeBufferMapSentry(m_traverse_backup_buffer.readBuffer(), RaBufferMapKind::read_write, 0);
    update_topology(static_cast<uint32_t>(mapping.address()[0]), 1U, 1U);


    RaRayGeneratorWithOutputBuffer::update(top_scene_object);
}

RaRecasterGenerator::~RaRecasterGenerator() = default;
