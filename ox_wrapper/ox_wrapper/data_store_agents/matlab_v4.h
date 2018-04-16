#ifndef OX_WRAPPER_DATA_STORE_AGENTS_MATLAB_V4_H
#define OX_WRAPPER_DATA_STORE_AGENTS_MATLAB_V4_H

#include "ox_wrapper/entity.h"
#include "ox_wrapper/buffer.h"

namespace ox_wrapper { namespace data_store_agents {

class OxMatlabV4 : public OxEntity
{
public:
    OxMatlabV4(std::string const& path, bool append = true);

public:
    // required by OxEntity interface
    bool isValid() const override;

public:

    bool save(OxAbstractBuffer const& source_buffer,
        uint32_t level,
        OxBasicBufferFormat source_buffer_format,
        std::string const& variable_name);

    bool load(OxAbstractBuffer& destination_buffer,
        uint32_t level,
        OxBasicBufferFormat destination_buffer_format,
        std::string const& variable_name);

private:
    std::string m_path;
    bool m_append;
};

}}

#endif
