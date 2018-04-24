#define _SCL_SECURE_NO_WARNINGS  

#include "matlab_v4.h"

#include "data_store_agents_common.h"
#include "ox_wrapper/util/misc.h"

#include <fstream>
#include <vector>
#include <cstddef>
#include <algorithm>

using namespace ox_wrapper;
using namespace ox_wrapper::data_store_agents;


namespace {

enum class MatlabV4NumericDataFormat : unsigned char
{
    double_precision_fp = 0,
    single_precision_fp,
    signed_32bit_integer,
    signed_16bit_integer,
    unsigned_16bit_integer,
    unsigned_8bit_integer
};

enum class MatlabV4MatrixType : unsigned char
{
    numeric = 0,
    text,
    sparse
};

enum class Endianness : unsigned char
{
    little_endian = 0,
    big_endian, 
    unknown
};

Endianness getEndianness()
{
    union {
        char bytes[4];
        uint32_t integer;
    }endiannes_check_helper;

    endiannes_check_helper.integer = 0x01020304;
    if (endiannes_check_helper.bytes[0] == 0x01
        && endiannes_check_helper.bytes[1] == 0x02
        && endiannes_check_helper.bytes[2] == 0x03
        && endiannes_check_helper.bytes[3] == 0x04)
    {
        return Endianness::big_endian;
    }
    else if (endiannes_check_helper.bytes[0] == 0x04
        && endiannes_check_helper.bytes[1] == 0x03
        && endiannes_check_helper.bytes[2] == 0x02
        && endiannes_check_helper.bytes[3] == 0x01)
    {
        return Endianness::little_endian;
    }

    return Endianness::unknown;
}

template<typename T>
T convertEndianness(Endianness target_endianness, T value)
{
    assert(target_endianness != Endianness::unknown);

    if (getEndianness() != target_endianness)
    {
        union coversion_helper {
            char data[sizeof(T)];
            T value;
        }helper;
        helper.value = value;

        for (size_t i = 0U; i < sizeof(T) / 2; ++i)
        {
            helper.data[i] = helper.data[sizeof(T) - 1 - i];
        }

        return helper.value;
    }
    else
        return value;
}

MatlabV4NumericDataFormat getNumericDataFormatFromMatlabV4Header(uint32_t header)
{
    header /= 10;
    return static_cast<MatlabV4NumericDataFormat>(header % 10);
}

MatlabV4MatrixType getMatrixTypeFromMatlabV4Header(uint32_t header)
{
    return static_cast<MatlabV4MatrixType>(header % 10);
}

void unpackHeader(uint32_t header, Endianness& endianness, MatlabV4NumericDataFormat& data_format, MatlabV4MatrixType& matrix_type)
{
    if (header / 1000 == 1) endianness = Endianness::big_endian;
    else if (header / 1000 == 0) endianness = Endianness::little_endian;
    else endianness = Endianness::unknown;
    header -= (header / 1000) * 1000;

    data_format = getNumericDataFormatFromMatlabV4Header(header);
    matrix_type = getMatrixTypeFromMatlabV4Header(header);
}

uint32_t packHeader(MatlabV4NumericDataFormat data_format, MatlabV4MatrixType matrix_type)
{
    Endianness endianness = getEndianness();
    if (endianness == Endianness::unknown)
        throw OxException{ "unknown host machine endianness" };

    uint32_t header = static_cast<uint32_t>(endianness) * 1000;
    header += static_cast<uint32_t>(data_format) * 10;
    header += static_cast<uint32_t>(matrix_type);

    return header;
}

bool findVariable(std::istream& input_stream, std::string const& variable_name,
    uint32_t& header, uint32_t& variable_num_rows, uint32_t& variable_num_columns,
    bool& is_complex, std::streamoff& variable_offset)
{
    input_stream.seekg(0, std::istream::beg);
    
    bool is_found = false;
    while (!is_found && input_stream)
    {
        input_stream.read(reinterpret_cast<char*>(&header), sizeof(uint32_t));

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
        else
        {
            size_t data_element_size{};
            switch (getNumericDataFormatFromMatlabV4Header(header))
            {
            case MatlabV4NumericDataFormat::double_precision_fp:
                data_element_size = 8U;
                break;

            case MatlabV4NumericDataFormat::single_precision_fp:
            case MatlabV4NumericDataFormat::signed_32bit_integer:
                data_element_size = 4U;
                break;

            case MatlabV4NumericDataFormat::signed_16bit_integer:
            case MatlabV4NumericDataFormat::unsigned_16bit_integer:
                data_element_size = 2U;
                break;

            case MatlabV4NumericDataFormat::unsigned_8bit_integer:
                data_element_size = 1U;
                break;
            }

            size_t data_to_skip = (is_complex ? 2U : 1U) * variable_num_rows * variable_num_columns * data_element_size;
            input_stream.seekg(data_to_skip, std::ios::cur);
        }
    }

    return is_found;
}

template<typename T> struct type_conversion_helper_c_to_matlab_v4;

template<> struct type_conversion_helper_c_to_matlab_v4<float>
{
    static constexpr MatlabV4NumericDataFormat format = MatlabV4NumericDataFormat::single_precision_fp;
    static constexpr MatlabV4MatrixType type = MatlabV4MatrixType::numeric;
};

template<> struct type_conversion_helper_c_to_matlab_v4<double>
{
    static constexpr MatlabV4NumericDataFormat format = MatlabV4NumericDataFormat::double_precision_fp;
    static constexpr MatlabV4MatrixType type = MatlabV4MatrixType::numeric;
};

template<> struct type_conversion_helper_c_to_matlab_v4<int32_t>
{
    static constexpr MatlabV4NumericDataFormat format = MatlabV4NumericDataFormat::signed_32bit_integer;
    static constexpr MatlabV4MatrixType type = MatlabV4MatrixType::numeric;
};

template<> struct type_conversion_helper_c_to_matlab_v4<int16_t>
{
    static constexpr MatlabV4NumericDataFormat format = MatlabV4NumericDataFormat::signed_16bit_integer;
    static constexpr MatlabV4MatrixType type = MatlabV4MatrixType::numeric;
};

template<> struct type_conversion_helper_c_to_matlab_v4<uint16_t>
{
    static constexpr MatlabV4NumericDataFormat format = MatlabV4NumericDataFormat::unsigned_16bit_integer;
    static constexpr MatlabV4MatrixType type = MatlabV4MatrixType::numeric;
};

template<> struct type_conversion_helper_c_to_matlab_v4<uint8_t>
{
    static constexpr MatlabV4NumericDataFormat format = MatlabV4NumericDataFormat::unsigned_8bit_integer;
    static constexpr MatlabV4MatrixType type = MatlabV4MatrixType::numeric;
};

template<typename T>
void writeVariable(std::ostream& stream, std::string const& variable_name, 
    uint32_t num_rows, uint32_t num_columns,
    std::vector<T> const& real_data, std::vector<T> const* p_imaginary_data = nullptr)
{
    if (real_data.size() != num_rows * num_columns 
        || p_imaginary_data && p_imaginary_data->size() != num_rows * num_columns)
        throw OxException{ ("Unable to write variable \"" + variable_name
            + "\" into MATLAB v4 file: dimension mismatch").c_str() };

    uint32_t header = packHeader(
        type_conversion_helper_c_to_matlab_v4<T>::format, 
        type_conversion_helper_c_to_matlab_v4<T>::type);
    stream.write(reinterpret_cast<char const*>(&header), sizeof(uint32_t));

    stream.write(reinterpret_cast<char const*>(&num_rows), sizeof(uint32_t));
    stream.write(reinterpret_cast<char const*>(&num_columns), sizeof(uint32_t));

    uint32_t imaginary = static_cast<uint32_t>(p_imaginary_data != nullptr);
    stream.write(reinterpret_cast<char const*>(&imaginary), sizeof(uint32_t));

    uint32_t name_length = static_cast<uint32_t>(variable_name.length() + 1);
    stream.write(reinterpret_cast<char const*>(&name_length), sizeof(uint32_t));
    stream.write(variable_name.c_str(), name_length);

    stream.write(reinterpret_cast<char const*>(real_data.data()), sizeof(T)*real_data.size());
    if (imaginary)
        stream.write(reinterpret_cast<char const*>(p_imaginary_data->data()), sizeof(T)*p_imaginary_data->size());
}


struct variable_info
{
    uint32_t num_rows;
    uint32_t num_columns;
    bool is_complex;
};


template<typename T>
std::pair<std::vector<T>, std::vector<T>> readVariable(std::istream& stream,
    std::string const& variable_name, variable_info& info)
{
    uint32_t header{};
    uint32_t num_rows{}, num_columns{};
    bool is_complex{};
    std::streamoff variable_offset{};

    if (!findVariable(stream, variable_name, header, 
        num_rows, num_columns, is_complex, variable_offset))
    {
        throw OxException{ "Unable to fetch data from MATLAB v4 variable \""
        + variable_name + "\": the variable cannot be found" };
    }

    info.num_rows = num_rows;
    info.num_columns = num_columns;
    info.is_complex = is_complex;

    Endianness endianness;
    MatlabV4NumericDataFormat data_format;
    MatlabV4MatrixType data_type;
    unpackHeader(header, endianness, data_format, data_type);

    if (data_format != type_conversion_helper_c_to_matlab_v4<T>::format)
    {
        throw OxException{ "Unable to fetch data from variable \"" + variable_name
        + "\": data format mismatch" };
    }

    auto fetch_data = 
        [num_rows, num_columns, endianness, data_format, data_type, &stream]
        (std::vector<T>& out)
    {
        out.resize(num_rows*num_columns);

        switch (data_type)
        {
        case MatlabV4MatrixType::numeric:
        {
            for (size_t i = 0U; i < num_rows*num_columns; ++i)
            {
                switch (data_format)
                {
                case MatlabV4NumericDataFormat::double_precision_fp:
                {
                    double aux;
                    stream.read(reinterpret_cast<char*>(&aux), sizeof(double));
                    out[i] = static_cast<T>(convertEndianness(endianness, aux));
                    break;
                }

                case MatlabV4NumericDataFormat::single_precision_fp:
                {
                    float aux;
                    stream.read(reinterpret_cast<char*>(&aux), sizeof(float));
                    out[i] = static_cast<T>(convertEndianness(endianness, aux));
                    break;
                }

                case MatlabV4NumericDataFormat::signed_32bit_integer:
                {
                    int32_t aux;
                    stream.read(reinterpret_cast<char*>(&aux), sizeof(int32_t));
                    out[i] = static_cast<T>(convertEndianness(endianness, aux));
                    break;
                }

                case MatlabV4NumericDataFormat::signed_16bit_integer:
                {
                    int16_t aux;
                    stream.read(reinterpret_cast<char*>(&aux), sizeof(int16_t));
                    out[i] = static_cast<T>(convertEndianness(endianness, aux));
                    break;
                }

                case MatlabV4NumericDataFormat::unsigned_16bit_integer:
                {
                    uint16_t aux;
                    stream.read(reinterpret_cast<char*>(&aux), sizeof(uint16_t));
                    out[i] = static_cast<T>(convertEndianness(endianness, aux));
                    break;
                }

                case MatlabV4NumericDataFormat::unsigned_8bit_integer:
                {
                    uint8_t aux;
                    stream.read(reinterpret_cast<char*>(&aux), sizeof(uint8_t));
                    out[i] = static_cast<T>(convertEndianness(endianness, aux));
                    break;
                }
                }
            }

            break;
        }

        case MatlabV4MatrixType::text:
            throw OxException{ "MATLAB v4 text variables are not supported" };

        case MatlabV4MatrixType::sparse:
            throw OxException{ "MATLAB v4 sparse variables are not supported" };
        }
    };

    std::pair<std::vector<T>, std::vector<T>> rv;
    
    stream.seekg(variable_offset, std::ios::beg);
    fetch_data(rv.first);
    if (is_complex) fetch_data(rv.second);

    return rv;
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
    {
        uint32_t const num_data_elements = num_rows * num_columns;
        auto mapping = makeBufferMapSentry(source_buffer, OxBufferMapKind::read, level);

        switch (source_buffer_format)
        {
        case OxBasicBufferFormat::FLOAT:
        {
            std::vector<float> src_data(num_data_elements);
            float const* p_raw_src_data = static_cast<float const*>(mapping.address());
            std::copy(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin());
            writeVariable(stream, variable_name, num_rows, num_columns, src_data);
            break;
        }
            
        case OxBasicBufferFormat::FLOAT2:
        {
            std::vector<float> src_data(2*num_data_elements);
            float2 const* p_raw_src_data = static_cast<float2 const*>(mapping.address());
            
            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[2 * i] = p_raw_src_data[i].x;
                src_data[2 * i + 1] = p_raw_src_data[i].y;
            }

            writeVariable(stream, variable_name, 2 * num_rows, num_columns, src_data);
            break;
        }
            
        case OxBasicBufferFormat::FLOAT3:
        {
            std::vector<float> src_data(3 * num_data_elements);
            float3 const* p_raw_src_data = static_cast<float3 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[3 * i] = p_raw_src_data[i].x;
                src_data[3 * i + 1] = p_raw_src_data[i].y;
                src_data[3 * i + 2] = p_raw_src_data[i].z;
            }

            writeVariable(stream, variable_name, 3 * num_rows, num_columns, src_data);
            break;
        }
            
        case OxBasicBufferFormat::FLOAT4:
        {
            std::vector<float> src_data(4 * num_data_elements);
            float4 const* p_raw_src_data = static_cast<float4 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[4 * i] = p_raw_src_data[i].x;
                src_data[4 * i + 1] = p_raw_src_data[i].y;
                src_data[4 * i + 2] = p_raw_src_data[i].z;
                src_data[4 * i + 3] = p_raw_src_data[i].w;
            }

            writeVariable(stream, variable_name, 4 * num_rows, num_columns, src_data);
            break;
        }
            
        case OxBasicBufferFormat::INT:
        {
            std::vector<int32_t> src_data(num_data_elements);
            int const* p_raw_src_data = static_cast<int const*>(mapping.address());
            std::transform(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin(),
                [](int e)->int32_t {return static_cast<int32_t>(e); });
            writeVariable(stream, variable_name, num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::INT2:
        {
            std::vector<int32_t> src_data(2 * num_data_elements);
            int2 const* p_raw_src_data = static_cast<int2 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[2 * i] = static_cast<int32_t>(p_raw_src_data[i].x);
                src_data[2 * i + 1] = static_cast<int32_t>(p_raw_src_data[i].y);
            }

            writeVariable(stream, variable_name, 2 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::INT3:
        {
            std::vector<int32_t> src_data(3 * num_data_elements);
            int3 const* p_raw_src_data = static_cast<int3 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[3 * i] = static_cast<int32_t>(p_raw_src_data[i].x);
                src_data[3 * i + 1] = static_cast<int32_t>(p_raw_src_data[i].y);
                src_data[3 * i + 2] = static_cast<int32_t>(p_raw_src_data[i].z);
            }

            writeVariable(stream, variable_name, 3 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::INT4:
        {
            std::vector<int32_t> src_data(4 * num_data_elements);
            int4 const* p_raw_src_data = static_cast<int4 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[4 * i] = static_cast<int32_t>(p_raw_src_data[i].x);
                src_data[4 * i + 1] = static_cast<int32_t>(p_raw_src_data[i].y);
                src_data[4 * i + 2] = static_cast<int32_t>(p_raw_src_data[i].z);
                src_data[4 * i + 3] = static_cast<int32_t>(p_raw_src_data[i].w);
            }

            writeVariable(stream, variable_name, 4 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UINT:
        {
            std::vector<int32_t> src_data(num_data_elements);
            unsigned int const* p_raw_src_data = static_cast<unsigned int const*>(mapping.address());
            std::transform(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin(),
                [](unsigned int e)->int32_t {return static_cast<int32_t>(e); });
            writeVariable(stream, variable_name, num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UINT2:
        {
            std::vector<int32_t> src_data(2 * num_data_elements);
            uint2 const* p_raw_src_data = static_cast<uint2 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[2 * i] = static_cast<int32_t>(p_raw_src_data[i].x);
                src_data[2 * i + 1] = static_cast<int32_t>(p_raw_src_data[i].y);
            }

            writeVariable(stream, variable_name, 2 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UINT3:
        {
            std::vector<int32_t> src_data(3 * num_data_elements);
            uint3 const* p_raw_src_data = static_cast<uint3 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[3 * i] = static_cast<int32_t>(p_raw_src_data[i].x);
                src_data[3 * i + 1] = static_cast<int32_t>(p_raw_src_data[i].y);
                src_data[3 * i + 2] = static_cast<int32_t>(p_raw_src_data[i].z);
            }

            writeVariable(stream, variable_name, 3 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UINT4:
        {
            std::vector<int32_t> src_data(4 * num_data_elements);
            uint4 const* p_raw_src_data = static_cast<uint4 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[4 * i] = static_cast<int32_t>(p_raw_src_data[i].x);
                src_data[4 * i + 1] = static_cast<int32_t>(p_raw_src_data[i].y);
                src_data[4 * i + 2] = static_cast<int32_t>(p_raw_src_data[i].z);
                src_data[4 * i + 3] = static_cast<int32_t>(p_raw_src_data[i].w);
            }

            writeVariable(stream, variable_name, 4 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::CHAR:
        {
            std::vector<uint8_t> src_data(num_data_elements);
            char const* p_raw_src_data = static_cast<char const*>(mapping.address());
            std::transform(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin(),
                [](char e)->uint8_t {return static_cast<uint8_t>(e); });
            writeVariable(stream, variable_name, num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::CHAR2:
        {
            std::vector<uint8_t> src_data(2 * num_data_elements);
            char2 const* p_raw_src_data = static_cast<char2 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[2 * i] = static_cast<uint8_t>(p_raw_src_data[i].x);
                src_data[2 * i + 1] = static_cast<uint8_t>(p_raw_src_data[i].y);
            }

            writeVariable(stream, variable_name, 2 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::CHAR3:
        {
            std::vector<uint8_t> src_data(3 * num_data_elements);
            char3 const* p_raw_src_data = static_cast<char3 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[3 * i] = static_cast<uint8_t>(p_raw_src_data[i].x);
                src_data[3 * i + 1] = static_cast<uint8_t>(p_raw_src_data[i].y);
                src_data[3 * i + 2] = static_cast<uint8_t>(p_raw_src_data[i].z);
            }

            writeVariable(stream, variable_name, 3 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::CHAR4:
        {
            std::vector<uint8_t> src_data(4 * num_data_elements);
            char4 const* p_raw_src_data = static_cast<char4 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[4 * i] = static_cast<uint8_t>(p_raw_src_data[i].x);
                src_data[4 * i + 1] = static_cast<uint8_t>(p_raw_src_data[i].y);
                src_data[4 * i + 2] = static_cast<uint8_t>(p_raw_src_data[i].z);
                src_data[4 * i + 3] = static_cast<uint8_t>(p_raw_src_data[i].w);
            }

            writeVariable(stream, variable_name, 4 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UCHAR:
        {
            std::vector<uint8_t> src_data(num_data_elements);
            unsigned char const* p_raw_src_data = static_cast<unsigned char const*>(mapping.address());
            std::transform(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin(),
                [](unsigned char e)->uint8_t {return static_cast<uint8_t>(e); });
            writeVariable(stream, variable_name, num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UCHAR2:
        {
            std::vector<uint8_t> src_data(2 * num_data_elements);
            uchar2 const* p_raw_src_data = static_cast<uchar2 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[2 * i] = static_cast<uint8_t>(p_raw_src_data[i].x);
                src_data[2 * i + 1] = static_cast<uint8_t>(p_raw_src_data[i].y);
            }

            writeVariable(stream, variable_name, 2 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UCHAR3:
        {
            std::vector<uint8_t> src_data(3 * num_data_elements);
            uchar3 const* p_raw_src_data = static_cast<uchar3 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[3 * i] = static_cast<uint8_t>(p_raw_src_data[i].x);
                src_data[3 * i + 1] = static_cast<uint8_t>(p_raw_src_data[i].y);
                src_data[3 * i + 2] = static_cast<uint8_t>(p_raw_src_data[i].z);
            }

            writeVariable(stream, variable_name, 3 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::UCHAR4:
        {
            std::vector<uint8_t> src_data(4 * num_data_elements);
            uchar4 const* p_raw_src_data = static_cast<uchar4 const*>(mapping.address());

            for (size_t i = 0U; i < num_data_elements; ++i)
            {
                src_data[4 * i] = static_cast<uint8_t>(p_raw_src_data[i].x);
                src_data[4 * i + 1] = static_cast<uint8_t>(p_raw_src_data[i].y);
                src_data[4 * i + 2] = static_cast<uint8_t>(p_raw_src_data[i].z);
                src_data[4 * i + 3] = static_cast<uint8_t>(p_raw_src_data[i].w);
            }

            writeVariable(stream, variable_name, 4 * num_rows, num_columns, src_data);
            break;
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        {
            {
                // write out spectral data

                uint8_t scale =
                    source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                    2 * constants::max_spectra_pairs_supported :
                    source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ? 2 : 1;

                std::vector<float> src_data(scale*num_data_elements);

                for (size_t i = 0; i < num_data_elements; ++i)
                {
                    if (source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD)
                    {
                        OxRayRadiancePayload const* p_data = 
                            static_cast<OxRayRadiancePayload const*>(mapping.address());
                        for (size_t j = 0; j < constants::max_spectra_pairs_supported; ++j)
                        {
                            src_data[scale*i + 2 * j] = p_data[i].spectral_radiance[j].x;
                            src_data[scale*i + 2 * j + 1] = p_data[i].spectral_radiance[j].y;
                        }
                    }
                    else if (source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE)
                    {
                        OxRayRadiancePayloadSimple const* p_data = 
                            static_cast<OxRayRadiancePayloadSimple const*>(mapping.address());
                        src_data[scale*i] = p_data[i].spectral_radiance.x;
                        src_data[scale*i + 1] = p_data[i].spectral_radiance.y;
                    }
                    else
                    {
                        OxRayRadiancePayloadMonochromatic const* p_data = 
                            static_cast<OxRayRadiancePayloadMonochromatic const*>(mapping.address());
                        src_data[scale*i] = p_data[i].spectral_radiance;
                    }
                }

                writeVariable(stream, variable_name + "__spectral_radiance", num_rows*scale, num_columns, src_data);
            }


            {
                size_t element_size =
                    source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                    sizeof(OxRayRadiancePayload) :
                    source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                    sizeof(OxRayRadiancePayloadSimple) : sizeof(OxRayRadiancePayloadMonochromatic);

                uint8_t const* p_raw_src_data = static_cast<uint8_t const*>(mapping.address());

                {
                    // write out depth data

                    size_t target_offset =
                        source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                        offsetof(OxRayRadiancePayload, depth) :
                        source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                        offsetof(OxRayRadiancePayloadSimple, depth) :
                        offsetof(OxRayRadiancePayloadMonochromatic, depth);

                    std::vector<float> src_data(2*num_data_elements);
                    for (size_t i = 0; i < num_data_elements; ++i)
                    {
                        float2 const* aux_ptr = reinterpret_cast<float2 const*>(p_raw_src_data + element_size * i + target_offset);
                        src_data[2 * i] = aux_ptr->x;
                        src_data[2 * i + 1] = aux_ptr->y;
                    }

                    writeVariable(stream, variable_name + "__depth", 2 * num_rows, num_columns, src_data);
                }

                {
                    // write out tracing recursion depth and auxiliary data
                    size_t target_offset =
                        source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                        offsetof(OxRayRadiancePayload, tracing_depth_and_aux) :
                        source_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                        offsetof(OxRayRadiancePayloadSimple, tracing_depth_and_aux) :
                        offsetof(OxRayRadiancePayloadMonochromatic, tracing_depth_and_aux);

                    std::vector<int32_t> src_data(4 * num_data_elements);
                    for (size_t i = 0; i < num_data_elements; ++i)
                    {
                        uint4 const* aux_ptr = reinterpret_cast<uint4 const*>(p_raw_src_data + element_size * i + target_offset);
                        src_data[4 * i] = static_cast<int32_t>(aux_ptr->x);
                        src_data[4 * i + 1] = static_cast<int32_t>(aux_ptr->y);
                        src_data[4 * i + 2] = static_cast<int32_t>(aux_ptr->z);
                        src_data[4 * i + 3] = static_cast<int32_t>(aux_ptr->w);
                    }

                    writeVariable(stream, variable_name + "__trace_recursion_depth_and_aux", 4 * num_rows, num_columns, src_data);
                }
            }

            break;
        }


        case OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        {
            OxRayOcclusionPayload const* p_raw_src_data =
                static_cast<OxRayOcclusionPayload const*>(mapping.address());

            {
                // write out occlusion mask

                std::vector<int32_t> src_data(num_data_elements);
                std::transform(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin(),
                    [](OxRayOcclusionPayload const& e)->int32_t {return static_cast<int32_t>(e.is_occluded); });
                writeVariable(stream, variable_name + "__is_occluded", num_rows, num_columns, src_data);
            }


            {
                // write out tracing recursion depth

                std::vector<int32_t> src_data(num_data_elements);
                std::transform(p_raw_src_data, p_raw_src_data + num_data_elements, src_data.begin(),
                    [](OxRayOcclusionPayload const& e)->int32_t {return static_cast<int32_t>(e.tracing_depth); });
                writeVariable(stream, variable_name + "__trace_recursion_depth", num_rows, num_columns, src_data);
            }

            {
                // write out depth data
                            /*write_preamble(MatlabV4NumericDataFormat::single_precision_fp, MatlabV4MatrixType::numeric,
                                2, variable_name + "__depth");*/
                std::vector<float> src_data(2 * num_data_elements);
                
                for (size_t i = 0U; i < num_data_elements; ++i)
                {
                    src_data[2 * i] = p_raw_src_data[i].depth.x;
                    src_data[2 * i + 1] = p_raw_src_data[i].depth.y;
                }

                writeVariable(stream, variable_name + "__is_occluded", 2 * num_rows, num_columns, src_data);
            }

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

    uint32_t dst_buffer_width = static_cast<uint32_t>(destination_buffer.getWidth());
    uint32_t dst_buffer_height = static_cast<uint32_t>(destination_buffer.getHeight());
    uint32_t dst_buffer_depth = static_cast<uint32_t>(destination_buffer.getDepth());

    auto mapping = makeBufferMapSentry(destination_buffer, OxBufferMapKind::write, level);

    try
    {
        switch (destination_buffer_format)
        {
        case OxBasicBufferFormat::FLOAT:
        case OxBasicBufferFormat::FLOAT2:
        case OxBasicBufferFormat::FLOAT3:
        case OxBasicBufferFormat::FLOAT4:
        {
            variable_info info;
            auto data = readVariable<float>(stream, variable_name, info);

            if (info.is_complex)
            {
                util::Log::retrieve()->out("Variable \"" + variable_name + "\" contains imaginary data "
                    "that are not supported", util::LogMessageType::error);
                return false;
            }

            uint8_t scale = static_cast<uint8_t>(destination_buffer_format)
                - static_cast<uint8_t>(OxBasicBufferFormat::FLOAT) + 1;
            if (info.num_rows / scale != dst_buffer_width
                || info.num_columns != dst_buffer_height * dst_buffer_depth)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                    "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / scale)
                    + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                    + std::to_string(dst_buffer_width) + " and height*depth="
                    + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                return false;
            }

            for (size_t i = 0U; i < info.num_rows / scale * info.num_columns; ++i)
            {
                switch (scale)
                {
                case 1:
                {
                    float* aux_ptr = static_cast<float*>(mapping.address());
                    aux_ptr[i] = data.first[i];
                    break;
                }

                case 2:
                {
                    float2* aux_ptr = static_cast<float2*>(mapping.address());
                    aux_ptr[i].x = data.first[2 * i];
                    aux_ptr[i].y = data.first[2 * i + 1];
                    break;
                }

                case 3:
                {
                    float3* aux_ptr = static_cast<float3*>(mapping.address());
                    aux_ptr[i].x = data.first[3 * i];
                    aux_ptr[i].y = data.first[3 * i + 1];
                    aux_ptr[i].z = data.first[3 * i + 2];
                    break;
                }

                case 4:
                {
                    float4* aux_ptr = static_cast<float4*>(mapping.address());
                    aux_ptr[i].x = data.first[4 * i];
                    aux_ptr[i].y = data.first[4 * i + 1];
                    aux_ptr[i].z = data.first[4 * i + 2];
                    aux_ptr[i].w = data.first[4 * i + 3];
                    break;
                }
                }
            }

            break;
        }

        case OxBasicBufferFormat::INT:
        case OxBasicBufferFormat::INT2:
        case OxBasicBufferFormat::INT3:
        case OxBasicBufferFormat::INT4:
        {
            variable_info info;
            auto data = readVariable<int32_t>(stream, variable_name, info);

            if (info.is_complex)
            {
                util::Log::retrieve()->out("Variable \"" + variable_name + "\" contains imaginary data "
                    "that are not supported", util::LogMessageType::error);
                return false;
            }

            uint8_t scale = static_cast<uint8_t>(destination_buffer_format)
                - static_cast<uint8_t>(OxBasicBufferFormat::INT) + 1;
            if (info.num_rows / scale != dst_buffer_width
                || info.num_columns != dst_buffer_height * dst_buffer_depth)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                    "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / scale)
                    + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                    + std::to_string(dst_buffer_width) + " and height*depth="
                    + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                return false;
            }

            for (size_t i = 0U; i < info.num_rows / scale * info.num_columns; ++i)
            {
                switch (scale)
                {
                case 1:
                {
                    int* aux_ptr = static_cast<int*>(mapping.address());
                    aux_ptr[i] = static_cast<int>(data.first[i]);
                    break;
                }

                case 2:
                {
                    int2* aux_ptr = static_cast<int2*>(mapping.address());
                    aux_ptr[i].x = static_cast<int>(data.first[2 * i]);
                    aux_ptr[i].y = static_cast<int>(data.first[2 * i + 1]);
                    break;
                }

                case 3:
                {
                    int3* aux_ptr = static_cast<int3*>(mapping.address());
                    aux_ptr[i].x = static_cast<int>(data.first[3 * i]);
                    aux_ptr[i].y = static_cast<int>(data.first[3 * i + 1]);
                    aux_ptr[i].z = static_cast<int>(data.first[3 * i + 2]);
                    break;
                }

                case 4:
                {
                    int4* aux_ptr = static_cast<int4*>(mapping.address());
                    aux_ptr[i].x = static_cast<int>(data.first[4 * i]);
                    aux_ptr[i].y = static_cast<int>(data.first[4 * i + 1]);
                    aux_ptr[i].z = static_cast<int>(data.first[4 * i + 2]);
                    aux_ptr[i].w = static_cast<int>(data.first[4 * i + 3]);
                    break;
                }
                }
            }

            break;
        }

        case OxBasicBufferFormat::UINT:
        case OxBasicBufferFormat::UINT2:
        case OxBasicBufferFormat::UINT3:
        case OxBasicBufferFormat::UINT4:
        {
            variable_info info;
            auto data = readVariable<int32_t>(stream, variable_name, info);

            if (info.is_complex)
            {
                util::Log::retrieve()->out("Variable \"" + variable_name + "\" contains imaginary data "
                    "that are not supported", util::LogMessageType::error);
                return false;
            }

            uint8_t scale = static_cast<uint8_t>(destination_buffer_format)
                - static_cast<uint8_t>(OxBasicBufferFormat::UINT) + 1;
            if (info.num_rows / scale != dst_buffer_width
                || info.num_columns != dst_buffer_height * dst_buffer_depth)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                    "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / scale)
                    + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                    + std::to_string(dst_buffer_width) + " and height*depth="
                    + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                return false;
            }

            for (size_t i = 0U; i < info.num_rows / scale * info.num_columns; ++i)
            {
                switch (scale)
                {
                case 1:
                {
                    unsigned int* aux_ptr = static_cast<unsigned int*>(mapping.address());
                    aux_ptr[i] = static_cast<unsigned int>(data.first[i]);
                    break;
                }

                case 2:
                {
                    uint2* aux_ptr = static_cast<uint2*>(mapping.address());
                    aux_ptr[i].x = static_cast<unsigned int>(data.first[2 * i]);
                    aux_ptr[i].y = static_cast<unsigned int>(data.first[2 * i + 1]);
                    break;
                }

                case 3:
                {
                    uint3* aux_ptr = static_cast<uint3*>(mapping.address());
                    aux_ptr[i].x = static_cast<unsigned int>(data.first[3 * i]);
                    aux_ptr[i].y = static_cast<unsigned int>(data.first[3 * i + 1]);
                    aux_ptr[i].z = static_cast<unsigned int>(data.first[3 * i + 2]);
                    break;
                }

                case 4:
                {
                    uint4* aux_ptr = static_cast<uint4*>(mapping.address());
                    aux_ptr[i].x = static_cast<unsigned int>(data.first[4 * i]);
                    aux_ptr[i].y = static_cast<unsigned int>(data.first[4 * i + 1]);
                    aux_ptr[i].z = static_cast<unsigned int>(data.first[4 * i + 2]);
                    aux_ptr[i].w = static_cast<unsigned int>(data.first[4 * i + 3]);
                    break;
                }
                }
            }

            break;
        }

        case OxBasicBufferFormat::CHAR:
        case OxBasicBufferFormat::CHAR2:
        case OxBasicBufferFormat::CHAR3:
        case OxBasicBufferFormat::CHAR4:
        {
            variable_info info;
            auto data = readVariable<uint8_t>(stream, variable_name, info);

            if (info.is_complex)
            {
                util::Log::retrieve()->out("Variable \"" + variable_name + "\" contains imaginary data "
                    "that are not supported", util::LogMessageType::error);
                return false;
            }

            uint8_t scale = static_cast<uint8_t>(destination_buffer_format)
                - static_cast<uint8_t>(OxBasicBufferFormat::CHAR) + 1;
            if (info.num_rows / scale != dst_buffer_width
                || info.num_columns != dst_buffer_height * dst_buffer_depth)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                    "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / scale)
                    + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                    + std::to_string(dst_buffer_width) + " and height*depth="
                    + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                return false;
            }

            for (size_t i = 0U; i < info.num_rows / scale * info.num_columns; ++i)
            {
                switch (scale)
                {
                case 1:
                {
                    char* aux_ptr = static_cast<char*>(mapping.address());
                    aux_ptr[i] = static_cast<char>(data.first[i]);
                    break;
                }

                case 2:
                {
                    char2* aux_ptr = static_cast<char2*>(mapping.address());
                    aux_ptr[i].x = static_cast<char>(data.first[2 * i]);
                    aux_ptr[i].y = static_cast<char>(data.first[2 * i + 1]);
                    break;
                }

                case 3:
                {
                    char3* aux_ptr = static_cast<char3*>(mapping.address());
                    aux_ptr[i].x = static_cast<char>(data.first[3 * i]);
                    aux_ptr[i].y = static_cast<char>(data.first[3 * i + 1]);
                    aux_ptr[i].z = static_cast<char>(data.first[3 * i + 2]);
                    break;
                }

                case 4:
                {
                    char4* aux_ptr = static_cast<char4*>(mapping.address());
                    aux_ptr[i].x = static_cast<char>(data.first[4 * i]);
                    aux_ptr[i].y = static_cast<char>(data.first[4 * i + 1]);
                    aux_ptr[i].z = static_cast<char>(data.first[4 * i + 2]);
                    aux_ptr[i].w = static_cast<char>(data.first[4 * i + 3]);
                    break;
                }
                }
            }

            break;
        }

        case OxBasicBufferFormat::UCHAR:
        case OxBasicBufferFormat::UCHAR2:
        case OxBasicBufferFormat::UCHAR3:
        case OxBasicBufferFormat::UCHAR4:
        {
            variable_info info;
            auto data = readVariable<uint8_t>(stream, variable_name, info);

            if (info.is_complex)
            {
                util::Log::retrieve()->out("Variable \"" + variable_name + "\" contains imaginary data "
                    "that are not supported", util::LogMessageType::error);
                return false;
            }

            uint8_t scale = static_cast<uint8_t>(destination_buffer_format)
                - static_cast<uint8_t>(OxBasicBufferFormat::UCHAR) + 1;
            if (info.num_rows / scale != dst_buffer_width
                || info.num_columns != dst_buffer_height * dst_buffer_depth)
            {
                util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                    + "\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                    "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / scale)
                    + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                    + std::to_string(dst_buffer_width) + " and height*depth="
                    + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                return false;
            }

            for (size_t i = 0U; i < info.num_rows / scale * info.num_columns; ++i)
            {
                switch (scale)
                {
                case 1:
                {
                    char* aux_ptr = static_cast<char*>(mapping.address());
                    aux_ptr[i] = static_cast<char>(data.first[i]);
                    break;
                }

                case 2:
                {
                    char2* aux_ptr = static_cast<char2*>(mapping.address());
                    aux_ptr[i].x = static_cast<char>(data.first[2 * i]);
                    aux_ptr[i].y = static_cast<char>(data.first[2 * i + 1]);
                    break;
                }

                case 3:
                {
                    char3* aux_ptr = static_cast<char3*>(mapping.address());
                    aux_ptr[i].x = static_cast<char>(data.first[3 * i]);
                    aux_ptr[i].y = static_cast<char>(data.first[3 * i + 1]);
                    aux_ptr[i].z = static_cast<char>(data.first[3 * i + 2]);
                    break;
                }

                case 4:
                {
                    char4* aux_ptr = static_cast<char4*>(mapping.address());
                    aux_ptr[i].x = static_cast<char>(data.first[4 * i]);
                    aux_ptr[i].y = static_cast<char>(data.first[4 * i + 1]);
                    aux_ptr[i].z = static_cast<char>(data.first[4 * i + 2]);
                    aux_ptr[i].w = static_cast<char>(data.first[4 * i + 3]);
                    break;
                }
                }
            }
        }

        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        case OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        {
            size_t element_size =
                destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                sizeof(OxRayRadiancePayload) :
                destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                sizeof(OxRayRadiancePayloadSimple) : sizeof(OxRayRadiancePayloadMonochromatic);

            {
                // read spectral radiance data

                variable_info info{};
                auto data = readVariable<float>(stream, variable_name + "__spectral_radiance", info);

                if (info.is_complex)
                {
                    util::Log::retrieve()->out("Variable \"" + variable_name + "__spectral_radiance\" contains imaginary data "
                        "that are not supported", util::LogMessageType::error);
                    return false;
                }

                uint8_t scale =
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                    2 * constants::max_spectra_pairs_supported :
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ? 2 : 1;

                if (info.num_rows / scale != dst_buffer_width
                    || info.num_columns != dst_buffer_height * dst_buffer_depth)
                {
                    util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                        + "__spectral_radiance\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                        "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / scale)
                        + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                        + std::to_string(dst_buffer_width) + " and height*depth="
                        + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                    return false;
                }


                size_t element_offset =
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                    offsetof(OxRayRadiancePayload, spectral_radiance) :
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                    offsetof(OxRayRadiancePayloadSimple, spectral_radiance) :
                    offsetof(OxRayRadiancePayloadMonochromatic, spectral_radiance);

                if (scale >= 2)
                {
                    for (size_t i = 0U; i < info.num_rows / scale * info.num_columns; ++i)
                    {
                        float2* aux_ptr = reinterpret_cast<float2*>(
                            static_cast<uint8_t*>(mapping.address()) + i * element_size + element_offset
                            );

                        for (size_t j = 0U; j < scale / 2; ++j)
                        {
                            aux_ptr[j].x = data.first[scale*i + 2 * j];
                            aux_ptr[j].y = data.first[scale*i + 2 * j + 1];
                        }
                    }
                }
                else
                {
                    std::copy(data.first.begin(), data.first.end(), static_cast<float*>(mapping.address()));
                }
            }

            {
                // read depth data

                variable_info info{};
                auto data = readVariable<float>(stream, variable_name + "__depth", info);

                if (info.is_complex)
                {
                    util::Log::retrieve()->out("Variable \"" + variable_name + "__depth\" contains imaginary data "
                        "that are not supported", util::LogMessageType::error);
                    return false;
                }

                if (info.num_rows / 2 != dst_buffer_width
                    || info.num_columns != dst_buffer_height * dst_buffer_depth)
                {
                    util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                        + "__depth\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                        "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / 2)
                        + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                        + std::to_string(dst_buffer_width) + " and height*depth="
                        + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                    return false;
                }


                size_t element_offset =
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                    offsetof(OxRayRadiancePayload, depth) :
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                    offsetof(OxRayRadiancePayloadSimple, depth) :
                    offsetof(OxRayRadiancePayloadMonochromatic, depth);

                for (size_t i = 0U; i < info.num_rows / 2 * info.num_columns; ++i)
                {
                    float2* aux_ptr = reinterpret_cast<float2*>(
                        static_cast<uint8_t*>(mapping.address()) + i * element_size + element_offset
                        );
                    aux_ptr->x = data.first[2 * i];
                    aux_ptr->y = data.first[2 * i + 1];
                }
            }

            {
                // read tracing recursion depth and auxiliary data

                variable_info info{};
                auto data = readVariable<int32_t>(stream, variable_name + "__trace_recursion_depth_and_aux", info);

                if (info.is_complex)
                {
                    util::Log::retrieve()->out("Variable \"" + variable_name + "__trace_recursion_depth_and_aux\" contains imaginary data "
                        "that are not supported", util::LogMessageType::error);
                    return false;
                }

                if (info.num_rows / 4 != dst_buffer_width
                    || info.num_columns != dst_buffer_height * dst_buffer_depth)
                {
                    util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                        + "__trace_recursion_depth_and_aux\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                        "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / 4)
                        + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                        + std::to_string(dst_buffer_width) + " and height*depth="
                        + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                    return false;
                }


                size_t element_offset =
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD ?
                    offsetof(OxRayRadiancePayload, tracing_depth_and_aux) :
                    destination_buffer_format == OxBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE ?
                    offsetof(OxRayRadiancePayloadSimple, tracing_depth_and_aux) :
                    offsetof(OxRayRadiancePayloadMonochromatic, tracing_depth_and_aux);

                for (size_t i = 0U; i < info.num_rows / 4 * info.num_columns; ++i)
                {
                    uint4* aux_ptr = reinterpret_cast<uint4*>(
                        static_cast<uint8_t*>(mapping.address()) + i * element_size + element_offset
                        );
                    aux_ptr->x = static_cast<unsigned int>(data.first[4 * i]);
                    aux_ptr->y = static_cast<unsigned int>(data.first[4 * i + 1]);
                    aux_ptr->z = static_cast<unsigned int>(data.first[4 * i + 2]);
                    aux_ptr->w = static_cast<unsigned int>(data.first[4 * i + 3]);
                }
            }

            break;
        }
        case OxBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        {
            OxRayOcclusionPayload* p_dst_buffer = static_cast<OxRayOcclusionPayload*>(mapping.address());
            
            {
                // read occlusion mask

                variable_info info{};
                auto data = readVariable<int32_t>(stream, variable_name + "__is_occluded", info);

                if (info.is_complex)
                {
                    util::Log::retrieve()->out("Variable \"" + variable_name + "__is_occluded\" contains imaginary data "
                        "that are not supported", util::LogMessageType::error);
                    return false;
                }

                if (info.num_rows != dst_buffer_width
                    || info.num_columns != dst_buffer_height * dst_buffer_depth)
                {
                    util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                        + "__is_occluded\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                        "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows)
                        + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                        + std::to_string(dst_buffer_width) + " and height*depth="
                        + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                    return false;
                }

                for (size_t i = 0U; i < info.num_rows * info.num_columns; ++i)
                {
                    p_dst_buffer[i].is_occluded = static_cast<unsigned int>(data.first[i]);
                }
            }

            {
                // read tracing recursion depth

                variable_info info{};
                auto data = readVariable<int32_t>(stream, variable_name + "__trace_recursion_depth", info);

                if (info.is_complex)
                {
                    util::Log::retrieve()->out("Variable \"" + variable_name + "__trace_recursion_depth\" contains imaginary data "
                        "that are not supported", util::LogMessageType::error);
                    return false;
                }

                if (info.num_rows != dst_buffer_width
                    || info.num_columns != dst_buffer_height * dst_buffer_depth)
                {
                    util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                        + "__trace_recursion_depth\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                        "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows)
                        + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                        + std::to_string(dst_buffer_width) + " and height*depth="
                        + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                    return false;
                }

                for (size_t i = 0U; i < info.num_rows * info.num_columns; ++i)
                {
                    p_dst_buffer[i].tracing_depth = static_cast<unsigned int>(data.first[i]);
                }
            }

            {
                // read depth data

                variable_info info{};
                auto data = readVariable<float>(stream, variable_name + "__depth", info);

                if (info.is_complex)
                {
                    util::Log::retrieve()->out("Variable \"" + variable_name + "__depth\" contains imaginary data "
                        "that are not supported", util::LogMessageType::error);
                    return false;
                }

                if (info.num_rows / 2 != dst_buffer_width
                    || info.num_columns != dst_buffer_height * dst_buffer_depth)
                {
                    util::Log::retrieve()->out("Unable to fetch data from variable \"" + variable_name
                        + "__depth\" into destination buffer \"" + destination_buffer.getStringName() + "\": "
                        "the buffer has invalid dimensions (expected width=" + std::to_string(info.num_rows / 2)
                        + " and height*depth=" + std::to_string(info.num_columns) + " but received width="
                        + std::to_string(dst_buffer_width) + " and height*depth="
                        + std::to_string(dst_buffer_height*dst_buffer_depth), util::LogMessageType::error);
                    return false;
                }

                for (size_t i = 0U; i < info.num_rows / 2 * info.num_columns; ++i)
                {
                    p_dst_buffer[i].depth.x = data.first[2 * i];
                    p_dst_buffer[i].depth.y = data.first[2 * i + 1];
                }
            }

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
    catch (OxException const&)
    {
        util::Log::retrieve()->out("Failure while fetching data from MATLAB v4 file \"" +
            m_path + "\"", util::LogMessageType::error);
        return false;
    }

    return true;
}



