#ifndef RA_DATA_STORE_AGENTS_MATLAB_V4_H
#define RA_DATA_STORE_AGENTS_MATLAB_V4_H

#include "entity.h"
#include "buffer.h"

#include <vector>

namespace ra { namespace data_store_agents {

class RaMatlabV4 : public RaEntity
{
public:
    RaMatlabV4(std::string const& path, bool append = true);

public:
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

    struct VariableInfo
    {
        std::string name;
        uint32_t num_rows;
        uint32_t num_columns;
        bool is_complex;
        MatlabV4MatrixType type;
        MatlabV4NumericDataFormat format;
    };

public:
    // required by RaEntity interface
    bool isValid() const override; 

public:

    bool save(RaAbstractBuffer const& source_buffer,
        uint32_t level,
        RaBasicBufferFormat source_buffer_format,
        std::string const& variable_name);

    bool load(RaAbstractBuffer& destination_buffer,
        uint32_t level,
        RaBasicBufferFormat destination_buffer_format,
        std::string const& variable_name);

    std::list<VariableInfo> getVariables() const;

private:
    std::string m_path;
    bool m_append;
    std::vector<VariableInfo> m_variables;
};

}}

#endif
