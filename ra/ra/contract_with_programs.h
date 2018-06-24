#ifndef OX_WRAPPER_CONTRACT_WITH_PROGRAMS_H
#define OX_WRAPPER_CONTRACT_WITH_PROGRAMS_H

#include "optix.h"

#include "ra/ra_fwd.h"
#include "ra/constants.h"
#include "ra/util/static_vector.h"
#include "ra/program.h"

#include <cstdint>
#include <initializer_list>


namespace ra {
    class OxContractWithOxPrograms 
    {
    public:
        OxProgram const* getProgramByName(std::string const& name) const;
        OxProgram const* getProgramById(OxEntityId const& id) const;

    protected:
        OxContractWithOxPrograms(std::initializer_list<OxProgram> init_list);

        uint32_t getDeclarationOffsetFromId(OxEntityId const& id) const;
        uint32_t getDeclarationOffsetFromName(std::string const& name) const;
        RTprogram nativeOptiXProgramHandle(uint32_t declaration_offset = 0U) const;
        OxProgram getOxProgramFromDeclarationOffset(uint32_t declaration_offset = 0U) const;
        size_t getAttachedProgramsCount() const;

        void replaceProgramAtOffset(OxProgram const& program, uint32_t declaration_offset = 0U);

    private:
        util::StaticVector<OxProgram, constants::max_programs_in_contract> m_programs;
    };
}

#endif
