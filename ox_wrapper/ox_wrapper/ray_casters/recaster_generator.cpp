#include "recaster_generator.h"

#include "../context.h"
#include "../../ptx.h"

using namespace ox_wrapper;
using namespace ox_wrapper::ray_casters;

namespace
{

std::string create_output_buffer_binding_name(OxAbstractBuffer const& output_buffer)
{
    switch (output_buffer.getDimension())
    {
    case OxBufferDimension::_1D:
        return "ox_radiance_payload_1d_out";

    case OxBufferDimension::_2D:
        return "ox_radiance_payload_2d_out";

    case OxBufferDimension::_3D:
        return "ox_radiance_payload_3d_out";

    default:
        throw OxException{ "unknown buffer dimension", __FILE__, __FUNCTION__, __LINE__ };
    }
}

}


OxRecasterGenerator::OxRecasterGenerator(
    OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayload> const& output_buffer, OxRayType recasted_ray_type):
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer), static_cast<uint32_t>(traverse_backup_buffer.getBufferPointer()[0]) },
    m_output_buffer{ output_buffer }
{
    getRayGenerationShader().setVariableValue("dimension", static_cast<unsigned int>(output_buffer.getDimension()));
    getRayGenerationShader().setVariableValue("payload_type", static_cast<unsigned int>(OxRayPayloadType::radiance));
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());
}

OxRecasterGenerator::OxRecasterGenerator(
    OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer, OxRayType recasted_ray_type) :
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer), static_cast<uint32_t>(traverse_backup_buffer.getBufferPointer()[0]) },
    m_output_buffer{ output_buffer }
{
    getRayGenerationShader().setVariableValue("dimension", static_cast<unsigned int>(output_buffer.getDimension()));
    getRayGenerationShader().setVariableValue("payload_type", static_cast<unsigned int>(OxRayPayloadType::radiance_simple));
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());

    switch (output_buffer.getDimension())
    {
    case OxBufferDimension::_1D:
        getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_1d_out", output_buffer);
        break;

    case OxBufferDimension::_2D:
        getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_2d_out", output_buffer);
        break;

    case OxBufferDimension::_3D:
        getRayGenerationShader().assignBuffer("ox_radiance_payload_simple_3d_out", output_buffer);
        break;
    }
}

OxRecasterGenerator::OxRecasterGenerator(
    OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer, OxRayType recasted_ray_type) :
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer), static_cast<uint32_t>(traverse_backup_buffer.getBufferPointer()[0]) },
    m_output_buffer{ output_buffer }
{
    getRayGenerationShader().setVariableValue("dimension", static_cast<unsigned int>(output_buffer.getDimension()));
    getRayGenerationShader().setVariableValue("payload_type", static_cast<unsigned int>(OxRayPayloadType::monochromatic));
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());

    switch (output_buffer.getDimension())
    {
    case OxBufferDimension::_1D:
        getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_1d_out", output_buffer);
        break;

    case OxBufferDimension::_2D:
        getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_2d_out", output_buffer);
        break;

    case OxBufferDimension::_3D:
        getRayGenerationShader().assignBuffer("ox_radiance_payload_monochromatic_3d_out", output_buffer);
        break;
    }
}

OxRecasterGenerator::OxRecasterGenerator(
    OxTraverseBackupBuffer const& traverse_backup_buffer, OxBuffer<OxRayOcclusionPayload> const& output_buffer, OxRayType recasted_ray_type) :
    OxRayGeneratorWithOutputBuffer{ 
    output_buffer.context().createProgram(PTX_RECASTER_GENERATOR, OxProgram::Source::file, OX_SHADER_ENTRY_RAY_GENERATION),
    output_buffer, create_output_buffer_binding_name(output_buffer), static_cast<uint32_t>(traverse_backup_buffer.getBufferPointer()[0]) },
    m_output_buffer{ output_buffer }
{
    getRayGenerationShader().setVariableValue("dimension", static_cast<unsigned int>(output_buffer.getDimension()));
    getRayGenerationShader().setVariableValue("payload_type", static_cast<unsigned int>(OxRayPayloadType::occlusion));
    getRayGenerationShader().setVariableValue("ray_type", static_cast<unsigned int>(recasted_ray_type));
    getRayGenerationShader().assignBuffer("traverse_backup_buffer", traverse_backup_buffer.getRawBuffer());

    switch (output_buffer.getDimension())
    {
    case OxBufferDimension::_1D:
        getRayGenerationShader().assignBuffer("ox_occlusion_payload_1d_out", output_buffer);
        break;

    case OxBufferDimension::_2D:
        getRayGenerationShader().assignBuffer("ox_occlusion_payload_2d_out", output_buffer);
        break;

    case OxBufferDimension::_3D:
        getRayGenerationShader().assignBuffer("ox_occlusion_payload_3d_out", output_buffer);
        break;
    }
}

OxAbstractBuffer const& OxRecasterGenerator::outputBuffer() const
{
    return m_output_buffer;
}

void OxRecasterGenerator::launch() const
{
    OxRayGenerator::update();
    OxRayGenerator::launch();
}
