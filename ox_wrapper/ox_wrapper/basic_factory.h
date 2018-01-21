#ifndef OX_WRAPPER_BASIC_FACTORY
#define OX_WRAPPER_BASIC_FACTORY

#include "entity.h"
#include "fwd.h"
#include "program.h"

namespace ox_wrapper {

class OxBasicFactory : public OxEntity
{
public:
    static OxBasicFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxBasicFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

    // programs
public:
    OxProgram createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const;

private:
    OxBasicFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}

#endif
