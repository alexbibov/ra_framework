#include "contract_with_programs.h"
#include "program.h"
#include "exception.h"

using namespace ra;


RaProgram const* RaContractWithRaPrograms::getProgramByName(std::string const& name) const
{
    for (auto& e : m_programs)
    {
        if (e.getStringName() == name) 
            return &e;
    }

    return nullptr;
}

RaProgram const* ra::RaContractWithRaPrograms::getProgramById(RaEntityId const& id) const
{
    for (auto& e : m_programs)
    {
        if (e.getUniqueIdentifier() == id)
            return &e;
    }

    return nullptr;
}

RaContractWithRaPrograms::RaContractWithRaPrograms(std::initializer_list<RaProgram> init_list):
    m_programs{ init_list }
{
}

uint32_t RaContractWithRaPrograms::getDeclarationOffsetFromId(RaEntityId const& id) const
{
    for (uint32_t i = 0; i < m_programs.size(); ++i)
    {
        if (m_programs[i].getUniqueIdentifier() == id)
            return i;
    }
    return static_cast<uint32_t>(-1);
}

uint32_t RaContractWithRaPrograms::getDeclarationOffsetFromName(std::string const& name) const
{
    for (uint32_t i = 0; i < m_programs.size(); ++i)
    {
        if (m_programs[i].getStringName() == name)
            return i;
    }
    return static_cast<uint32_t>(-1);
}

RTprogram RaContractWithRaPrograms::nativeOptiXProgramHandle(uint32_t declaration_offset) const
{
    return m_programs[declaration_offset].m_native_optix_program.get();
}

RaProgram RaContractWithRaPrograms::getRaProgramFromDeclarationOffset(uint32_t declaration_offset) const
{
    return m_programs[declaration_offset];
}

size_t RaContractWithRaPrograms::getAttachedProgramsCount() const
{
    return m_programs.size();
}

void RaContractWithRaPrograms::replaceProgramAtOffset(RaProgram const& program, uint32_t declaration_offset)
{
    RaProgram& p = m_programs[declaration_offset];
    if (p.getUniqueIdentifier() != program.getUniqueIdentifier())
        p = program;
}
