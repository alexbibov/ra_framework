#include "matlab_v4.h"

#include "data_store_agents_common.h"
#include "ox_wrapper/util/misc.h"

#include <fstream>
#include <vector>

using namespace ox_wrapper;
using namespace ox_wrapper::data_store_agents;


namespace {

bool findVariable(std::istream& input_stream, std::string const& variable_name,
    uint32_t& variable_num_rows, uint32_t& variable_num_columns, 
    bool& is_complex, std::streamoff& variable_offset)
{
    input_stream.seekg(0, std::istream::beg);
    {
        uint32_t magic_bytes{};
        input_stream.read(reinterpret_cast<char*>(&magic_bytes), sizeof(uint32_t));

        if (magic_bytes) return false;    // invalid file format
    }

    bool is_found = false;
    while (!is_found && input_stream)
    {
        input_stream.read(reinterpret_cast<char*>(&variable_num_rows), sizeof(uint32_t));
        input_stream.read(reinterpret_cast<char*>(&variable_num_columns), sizeof(uint32_t));

        uint32_t imaginary{};
        input_stream.read(reinterpret_cast<char*>(&imaginary), sizeof(uint32_t));

        uint32_t variable_name_length{};
        input_stream.read(reinterpret_cast<char*>(&variable_name_length), sizeof(uint32_t));

        std::vector<char> var_name(variable_name_length);
        input_stream.read(var_name.data(), variable_name_length);

        if (std::string{ var_name.begin(), var_name.end() } == variable_name)
        {
            is_complex = imaginary != 0;
            auto current_pos = input_stream.tellg();
            input_stream.seekg(0, std::istream::beg);
            variable_offset = current_pos - input_stream.tellg();
            is_found = true;
        }
    }

    return is_found;
}

}


OxMatlabV4::OxMatlabV4(std::string const& path, bool append):
    m_path{ path },
    m_append{ append }
{
    
}

bool OxMatlabV4::isValid() const
{
    return true;
}


bool OxMatlabV4::save(OxAbstractBuffer const& source_buffer, uint32_t level,
    OxBasicBufferFormat source_buffer_format, std::string const& variable_name)
{
    auto stream_flags = std::ios_base::out | std::ios_base::binary;
    bool file_exists = util::misc::doesFileExist(m_path);

    if (!file_exists || !m_append)
        stream_flags |= std::ios_base::trunc;

    std::fstream stream{ m_path, stream_flags };
    if (!stream) return false;

    // not that data is stored in the destination variable in transposed form
    uint32_t num_rows = static_cast<uint32_t>(source_buffer.getWidth());
    uint32_t height = static_cast<uint32_t>(source_buffer.getHeight());
    uint32_t depth = static_cast<uint32_t>(source_buffer.getDepth());
    uint32_t num_columns = height * depth;

    uint32_t magic_bytes = 0;
    stream.write(reinterpret_cast<char const*>(&magic_bytes), sizeof(magic_bytes));

    stream.write(reinterpret_cast<char const*>(&num_rows), sizeof(uint32_t));
    stream.write(reinterpret_cast<char const*>(&num_columns), sizeof(uint32_t));

    {
        uint32_t imaginary = 0U;    // buffers with complex data are not currently supported
        stream.write(reinterpret_cast<char const*>(&imaginary), sizeof(uint32_t));
    }

    {
        uint32_t name_length = static_cast<uint32_t>(variable_name.length()) + 1;
        stream.write(reinterpret_cast<char const*>(&name_length), sizeof(uint32_t));
        stream.write(variable_name.c_str(), name_length);
    }

    {
        uint32_t const num_data_elements = num_rows * num_columns;

        switch (source_buffer_format)
        {
        case OxBasicBufferFormat::FLOAT:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT>::
                write(stream, static_cast<float const*>(buffer_mapper.address()), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::FLOAT2:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT2>::
                write(stream, static_cast<float2 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::FLOAT3:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT3>::
                write(stream, static_cast<float3 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::FLOAT4:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT4>::
                write(stream, static_cast<float4 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::INT:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT>::
                write(stream, static_cast<int const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT2:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT2>::
                write(stream, static_cast<int2 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT3:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT3>::
                write(stream, static_cast<int3 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT4:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT4>::
                write(stream, static_cast<int4 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT>::
                write(stream, static_cast<unsigned int const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT2:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT2>::
                write(stream, static_cast<uint2 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT3:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT3>::
                write(stream, static_cast<uint3 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT4:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT4>::
                write(stream, static_cast<uint4 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR>::
                write(stream, static_cast<char const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR2:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR2>::
                write(stream, static_cast<char2 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR3:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR3>::
                write(stream, static_cast<char3 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR4:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR4>::
                write(stream, static_cast<char4 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR>::
                write(stream, static_cast<unsigned char const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR2:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR2>::
                write(stream, static_cast<uchar2 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR3:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR3>::
                write(stream, static_cast<uchar3 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR4:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR4>::
                write(stream, static_cast<uchar4 const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD>::
                write(stream, static_cast<OxRayRadiancePayload const*>(buffer_mapper.address()), num_data_elements);
            
            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE>::
                write(stream, static_cast<OxRayRadiancePayloadSimple const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC>::
                write(stream, static_cast<OxRayRadiancePayloadMonochromatic const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        {
            auto buffer_mapper = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD>::
                write(stream, static_cast<OxRayOcclusionPayload const*>(buffer_mapper.address()), num_data_elements);

            break;
        }

        default:
            util::Log::retrieve()->out("Unable to save data from buffer \"" + source_buffer.getStringName()
                + "\" into destination MATLAB v4 file \"" + m_path + "\": the buffer has a non-standard format",
                util::LogMessageType::error);

            return false;
        }

        return true;
    }
}

bool OxMatlabV4::load(OxAbstractBuffer& destination_buffer, uint32_t level, OxBasicBufferFormat destination_buffer_format, std::string const& variable_name)
{
    std::fstream stream{ m_path, std::ios_base::in | std::ios_base::binary };
    if (!stream) return false;

    uint32_t magic_bytes;
    stream.read(reinterpret_cast<char*>(&magic_bytes), sizeof(magic_bytes));

    if (magic_bytes != 0)
    {
        util::Log::retrieve()->out("File \"" + m_path + "\" attempted to be loaded as MATLAB v4 file "
            "appears to have invalid format or is damaged", util::LogMessageType::error);
        return false;
    }


    uint32_t num_rows{}, num_columns{};
    {
        uint32_t dst_buffer_width = static_cast<uint32_t>(destination_buffer.getWidth());
        uint32_t dst_buffer_height = static_cast<uint32_t>(destination_buffer.getHeight());
        uint32_t dst_buffer_depth = static_cast<uint32_t>(destination_buffer.getDepth());

        bool is_complex{};
        std::streamoff variable_offset{};

        if (!findVariable(stream, variable_name, num_rows, num_columns, is_complex, variable_offset))
        {
            util::Log::retrieve()->out("Unable to fetch data from variable \""
                + variable_name + "\" requested from MATLAB v4 file \"" + m_path
                + "\": the variable could not be found", util::LogMessageType::error);
            return false;
        }

        if (num_rows != dst_buffer_width || num_columns != (dst_buffer_height * dst_buffer_depth))
        {
            util::Log::retrieve()->out("Unable to fetch data from variable \""
                + variable_name + "\" in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                + destination_buffer.getStringName() + "\": the buffer is having invalid dimensions"
                + " (expected width=" + std::to_string(num_rows) + " and height*depth="
                + std::to_string(num_columns) + ", but received width=" + std::to_string(dst_buffer_width)
                + " and height*depth=" + std::to_string(dst_buffer_height*dst_buffer_depth) + ")",
                util::LogMessageType::error);
            return false;
        }

        if (is_complex)
        {
            util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                + "\" located in MATLAB v4 file \"" + m_path + "\": the variable seems to contain "
                "imaginary data, which is currently not supported", util::LogMessageType::error);
            return false;
        }

        stream.seekg(variable_offset, std::istream::beg);
    }

    

    {
        uint32_t const num_data_elements = num_rows * num_columns;

        switch (destination_buffer_format)
        {
        case OxBasicBufferFormat::FLOAT:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT>::
                read(stream, static_cast<float*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::FLOAT2:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT2>::
                read(stream, static_cast<float2*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::FLOAT3:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT3>::
                read(stream, static_cast<float3*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::FLOAT4:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT4>::
                read(stream, static_cast<float4*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT>::
                read(stream, static_cast<int*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT2:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT2>::
                read(stream, static_cast<int2*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT3:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT3>::
                read(stream, static_cast<int3*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT4:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT4>::
                read(stream, static_cast<int4*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT>::
                read(stream, static_cast<unsigned int*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT2:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT2>::
                read(stream, static_cast<uint2*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT3:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT3>::
                read(stream, static_cast<uint3*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT4:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT4>::
                read(stream, static_cast<uint4*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR>::
                read(stream, static_cast<char*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR2:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR2>::
                read(stream, static_cast<char2*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR3:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR3>::
                read(stream, static_cast<char3*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR4:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR4>::
                read(stream, static_cast<char4*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR>::
                read(stream, static_cast<unsigned char*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR2:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR2>::
                read(stream, static_cast<uchar2*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR3:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR3>::
                read(stream, static_cast<uchar3*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR4:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR4>::
                read(stream, static_cast<uchar4*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD>::
                read(stream, static_cast<OxRayRadiancePayload*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE>::
                read(stream, static_cast<OxRayRadiancePayloadSimple*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC>::
                read(stream, static_cast<OxRayRadiancePayloadMonochromatic*>(mapping.address()), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        {
            auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD>::
                read(stream, static_cast<OxRayOcclusionPayload*>(mapping.address()), num_data_elements);

            break;
        }

        default:
            util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                + "\" located in MATLAB v4 file \"" + m_path + "\": the destination buffer \""
                + destination_buffer.getStringName() + "\" has invalid format",
                util::LogMessageType::error);

            return false;
        }
    }


    return true;
}
