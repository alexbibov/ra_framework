#ifndef OX_WRAPPER_DATA_STORE_AGENTS_FACTORY_H
#define OX_WRAPPER_DATA_STORE_AGENTS_FACTORY_H

#include "ra/entity.h"
#include "ra/ra_fwd.h"

#include "matlab_v4.h"

namespace ra { namespace data_store_agents {

class OxDataStoreAgentsFactory : public OxEntity
{
public:
    static OxDataStoreAgentsFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxDataStoreAgentsFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

public:
    OxMatlabV4 createMatlabV4Agent(std::string const& target_path, bool append_data = true);

private:
    OxDataStoreAgentsFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}}

#endif
