#include "matlab_v4.h"

#include "data_store_agents_common.h"
#include "ox_wrapper/util/misc.h"

#include <fstream>

using namespace ox_wrapper;
using namespace ox_wrapper::data_store_agents;

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

    uint32_t width = static_cast<uint32_t>(source_buffer.getWidth());
    uint32_t height = static_cast<uint32_t>(source_buffer.getHeight());
    uint32_t depth = static_cast<uint32_t>(source_buffer.getDepth());
    uint32_t reshaped_height = height * depth;

    stream.write(reinterpret_cast<char const*>(&reshaped_height), sizeof(uint32_t));
    stream.write(reinterpret_cast<char const*>(&width), sizeof(uint32_t));

    {
        uint32_t imaginary = false;    // buffers with complex data are not currently supported
        stream.write(reinterpret_cast<char const*>(&imaginary), sizeof(uint32_t));
    }

    {
        uint32_t name_length = static_cast<uint32_t>(variable_name.length());
        stream.write(reinterpret_cast<char const*>(&name_length), sizeof(uint32_t));
    }


    auto buffer_mapping = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);
    {
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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);
            
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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

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
                write(stream, buffer_mapper.address(), width*reshaped_height);

            break;
        }

        }

        return true;
    }
}

bool MatlabV4::load(OxAbstractBuffer& destination_buffer, uint32_t level, OxBasicBufferFormat destination_buffer_format, std::string const& variable_name)
{
    return true;
}
