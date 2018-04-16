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


MatlabV4::MatlabV4(std::string const& path, bool append):
    m_path{ path },
    m_append{ append }
{
    
}


MatlabV4::~MatlabV4() = default;


bool MatlabV4::save(OxAbstractBuffer const& source_buffer, uint32_t level,
    OxBasicBufferFormat source_buffer_format, std::string const& variable_name)
{
    auto stream_flags = std::ios_base::out | std::ios_base::binary;
    bool file_exists = util::misc::doesFileExist(m_path);

    if (!file_exists || m_append)
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
            OxBuffer<float> const* p_buf = dynamic_cast<OxBuffer<float> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \"" 
                    + m_path + "\": the standard data format specified for the buffer (FLOAT) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::FLOAT2:
        {
            OxBuffer<float2> const* p_buf = dynamic_cast<OxBuffer<float2> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (FLOAT2) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT2>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::FLOAT3:
        {
            OxBuffer<float3> const* p_buf = dynamic_cast<OxBuffer<float3> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (FLOAT3) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT3>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::FLOAT4:
        {
            OxBuffer<float4> const* p_buf = dynamic_cast<OxBuffer<float4> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (FLOAT4) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT4>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }
            
        case OxBasicBufferFormat::INT:
        {
            OxBuffer<int> const* p_buf = dynamic_cast<OxBuffer<int> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (INT) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT2:
        {
            OxBuffer<int2> const* p_buf = dynamic_cast<OxBuffer<int2> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (INT2) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT2>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT3:
        {
            OxBuffer<int3> const* p_buf = dynamic_cast<OxBuffer<int3> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (INT3) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT3>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT4:
        {
            OxBuffer<int4> const* p_buf = dynamic_cast<OxBuffer<int4> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (INT4) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT4>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT:
        {
            OxBuffer<unsigned int> const* p_buf = dynamic_cast<OxBuffer<unsigned int> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UINT) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT2:
        {
            OxBuffer<uint2> const* p_buf = dynamic_cast<OxBuffer<uint2> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UINT2) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT2>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT3:
        {
            OxBuffer<uint3> const* p_buf = dynamic_cast<OxBuffer<uint3> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UINT3) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT3>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT4:
        {
            OxBuffer<uint4> const* p_buf = dynamic_cast<OxBuffer<uint4> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UINT4) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT4>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR:
        {
            OxBuffer<char> const* p_buf = dynamic_cast<OxBuffer<char> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (CHAR) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR2:
        {
            OxBuffer<char2> const* p_buf = dynamic_cast<OxBuffer<char2> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (CHAR2) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR2>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR3:
        {
            OxBuffer<char3> const* p_buf = dynamic_cast<OxBuffer<char3> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (CHAR3) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR3>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR4:
        {
            OxBuffer<char4> const* p_buf = dynamic_cast<OxBuffer<char4> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (CHAR4) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR4>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR:
        {
            OxBuffer<unsigned char> const* p_buf = dynamic_cast<OxBuffer<unsigned char> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UCHAR) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR2:
        {
            OxBuffer<uchar2> const* p_buf = dynamic_cast<OxBuffer<uchar2> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UCHAR2) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR2>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR3:
        {
            OxBuffer<uchar3> const* p_buf = dynamic_cast<OxBuffer<uchar3> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UCHAR3) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR3>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR4:
        {
            OxBuffer<uchar4> const* p_buf = dynamic_cast<OxBuffer<uchar4> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (UCHAR4) does not correspond to "
                    "the actual data stored in the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR4>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        {
            OxBuffer<OxRayRadiancePayload> const* p_buf = 
                dynamic_cast<OxBuffer<OxRayRadiancePayload> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (RAY_RADIANCE_PAYLOAD) does not correspond to "
                    "the actual data stored int the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD>::
                write(stream, buffer_mapper.address(), num_data_elements);
            
            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        {
            OxBuffer<OxRayRadiancePayloadSimple> const* p_buf = 
                dynamic_cast<OxBuffer<OxRayRadiancePayloadSimple> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (RAY_RADIANCE_PAYLOAD_SIMPLE) does not correspond to "
                    "the actual data stored int the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        {
            OxBuffer<OxRayRadiancePayloadMonochromatic> const* p_buf =
                dynamic_cast<OxBuffer<OxRayRadiancePayloadMonochromatic> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (RAY_RADIANCE_PAYLOAD_MONOCHROMATIC) does not correspond to "
                    "the actual data stored int the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC>::
                write(stream, buffer_mapper.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        {
            OxBuffer<OxRayOcclusionPayload> const* p_buf =
                dynamic_cast<OxBuffer<OxRayOcclusionPayload> const*>(&source_buffer);
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to write data from buffer \""
                    + source_buffer.getStringName() + "\" to MATLAB v4 file \""
                    + m_path + "\": the standard data format specified for the buffer (RAY_OCCLUSION_PAYLOAD) does not correspond to "
                    "the actual data stored int the buffer", util::LogMessageType::error);
                return false;
            }

            auto buffer_mapper = makeBufferMapSentry(*p_buf, OxBufferMapKind::read, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD>::
                write(stream, buffer_mapper.address(), num_data_elements);

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

bool MatlabV4::load(OxAbstractBuffer& destination_buffer, uint32_t level, OxBasicBufferFormat destination_buffer_format, std::string const& variable_name)
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
            OxBuffer<float>* p_buf = dynamic_cast<OxBuffer<float>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName() 
                    + "\": the buffer appears to have invalid format (FLOAT was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::FLOAT2:
        {
            OxBuffer<float2>* p_buf = dynamic_cast<OxBuffer<float2>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName() 
                    + "\": the buffer appears to have invalid format (FLOAT2 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT2>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::FLOAT3:
        {
            OxBuffer<float3>* p_buf = dynamic_cast<OxBuffer<float3>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (FLOAT3 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT3>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::FLOAT4:
        {
            OxBuffer<float4>* p_buf = dynamic_cast<OxBuffer<float4>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (FLOAT4 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::FLOAT4>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT:
        {
            OxBuffer<int>* p_buf = dynamic_cast<OxBuffer<int>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (INT was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT2:
        {
            OxBuffer<int2>* p_buf = dynamic_cast<OxBuffer<int2>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (INT2 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT2>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT3:
        {
            OxBuffer<int3>* p_buf = dynamic_cast<OxBuffer<int3>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (INT3 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT3>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::INT4:
        {
            OxBuffer<int4>* p_buf = dynamic_cast<OxBuffer<int4>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (INT4 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::INT4>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT:
        {
            OxBuffer<unsigned int>* p_buf = dynamic_cast<OxBuffer<unsigned int>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UINT was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT2:
        {
            OxBuffer<uint2>* p_buf = dynamic_cast<OxBuffer<uint2>*>(&destination_buffer);
           
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UINT2 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT2>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT3:
        {
            OxBuffer<uint3>* p_buf = dynamic_cast<OxBuffer<uint3>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UINT3 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT3>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UINT4:
        {
            OxBuffer<uint4>* p_buf = dynamic_cast<OxBuffer<uint4>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UINT4 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UINT4>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR:
        {
            OxBuffer<char>* p_buf = dynamic_cast<OxBuffer<char>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (CHAR was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR2:
        {
            OxBuffer<char2>* p_buf = dynamic_cast<OxBuffer<char2>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (CHAR2 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR2>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR3:
        {
            OxBuffer<char3>* p_buf = dynamic_cast<OxBuffer<char3>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (CHAR3 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR3>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::CHAR4:
        {
            OxBuffer<char4>* p_buf = dynamic_cast<OxBuffer<char4>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (CHAR4 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::CHAR4>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR:
        {
            OxBuffer<unsigned char>* p_buf = dynamic_cast<OxBuffer<unsigned char>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UCHAR was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR2:
        {
            OxBuffer<uchar2>* p_buf = dynamic_cast<OxBuffer<uchar2>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UCHAR2 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR2>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR3:
        {
            OxBuffer<uchar3>* p_buf = dynamic_cast<OxBuffer<uchar3>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UCHAR3 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR3>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::UCHAR4:
        {
            OxBuffer<uchar4>* p_buf = dynamic_cast<OxBuffer<uchar4>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (UCHAR4 was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::UCHAR4>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        {
            OxBuffer<OxRayRadiancePayload>* p_buf 
                = dynamic_cast<OxBuffer<OxRayRadiancePayload>*>(&destination_buffer);
            
            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (RAY_RADIANCE_PAYLOAD was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        {
            OxBuffer<OxRayRadiancePayloadSimple>* p_buf 
                = dynamic_cast<OxBuffer<OxRayRadiancePayloadSimple>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (RAY_RADIANCE_PAYLOAD_SIMPLE was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        {
            OxBuffer<OxRayRadiancePayloadMonochromatic>* p_buf
                = dynamic_cast<OxBuffer<OxRayRadiancePayloadMonochromatic>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (RAY_RADIANCE_PAYLOAD_MONOCHROMATIC was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC>::
                read(stream, mapping.address(), num_data_elements);

            break;
        }

        case OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        {
            OxBuffer<OxRayOcclusionPayload>* p_buf
                = dynamic_cast<OxBuffer<OxRayOcclusionPayload>*>(&destination_buffer);

            if (p_buf == nullptr)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" located in MATLAB v4 file \"" + m_path + "\" into destination buffer \""
                    + destination_buffer.getStringName()
                    + "\": the buffer appears to have invalid format (RAY_OCCLUSION_PAYLOAD was expected)",
                    util::LogMessageType::error);

                return false;
            }

            auto mapping = makeBufferMapSentry(*p_buf, OxBufferMapKind::write, level);
            DataReadWriteHelper<OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD>::
                read(stream, mapping.address(), num_data_elements);

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
