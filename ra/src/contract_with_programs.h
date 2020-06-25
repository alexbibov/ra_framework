#ifndef RA_CONTRACT_WITH_PROGRAMS_H
#define RA_CONTRACT_WITH_PROGRAMS_H

#include "optix.h"

#include "ra_fwd.h"
#include "constants.h"
#include "util/static_vector.h"
#include "program.h"

#include <cstdint>
#include <initializer_list>


namespace ra {
    class RaContractWithRaPrograms 
    {
    public:
        RaProgram const* getProgramByName(std::string const& name) const;
        RaProgram const* getProgramById(RaEntityId const& id) const;

    protected:
        RaContractWithRaPrograms(std::initializer_list<RaProgram> init_list);

        uint32_t getDeclarationOffsetFromId(RaEntityId const& id) const;
        uint32_t getDeclarationOffsetFromName(std::string const& name) const;
        RTprogram nativeOptiXProgramHandle(uint32_t declaration_offset = 0U) const;
        RaProgram getRaProgramFromDeclarationOffset(uint32_t declaration_offset = 0U) const;
        size_t getAttachedProgramsCount() const;

        void replaceProgramAtOffset(RaProgram const& program, uint32_t declaration_offset = 0U);

    private:
        util::StaticVector<RaProgram, constants::max_programs_in_contract> m_programs;
    };
}

#endif
