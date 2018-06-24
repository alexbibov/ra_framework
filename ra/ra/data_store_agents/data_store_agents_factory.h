#ifndef RA_DATA_STORE_AGENTS_FACTORY_H
#define RA_DATA_STORE_AGENTS_FACTORY_H

#include "ra/entity.h"
#include "ra/ra_fwd.h"

#include "matlab_v4.h"

namespace ra { namespace data_store_agents {

class RaDataStoreAgentsFactory : public RaEntity
{
public:
    static RaDataStoreAgentsFactory* initialize(RaContext const& context);
    static void shutdown();
    static RaDataStoreAgentsFactory* retrieve();

public:
    // required by RaEntity interface
    bool isValid() const override;

public:
    RaMatlabV4 createMatlabV4Agent(std::string const& target_path, bool append_data = true);

private:
    RaDataStoreAgentsFactory(RaContext const& context);

private:
    RaContext const& m_context;
};

}}

#endif
