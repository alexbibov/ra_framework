#include "contract_with_programs.h"
#include "program.h"
#include "exception.h"

using namespace ox_wrapper;


OxProgram const* OxContractWithOxPrograms::getProgramByName(std::string const& name) const
{
    for (auto& e : m_programs)
    {
        if (e.getStringName() == name) 
            return &e;
    }

    return nullptr;
}

OxProgram const* ox_wrapper::OxContractWithOxPrograms::getProgramById(OxEntityID const& id) const
{
    for (auto& e : m_programs)
    {
        if (e.getUniqueIdentifier() == id)
            return &e;
    }

    return nullptr;
}

OxContractWithOxPrograms::OxContractWithOxPrograms(std::initializer_list<OxProgram> init_list):
    m_programs{ init_list }
{
}

uint32_t OxContractWithOxPrograms::getDeclarationOffsetFromId(OxEntityID const& id) const
{
    for (uint32_t i = 0; i < m_programs.size(); ++i)
    {
        if (m_programs[i].getUniqueIdentifier() == id)
            return i;
    }
    return static_cast<uint32_t>(-1);
}

uint32_t OxContractWithOxPrograms::getDeclarationOffsetFromName(std::string const& name) const
{
    for (uint32_t i = 0; i < m_programs.size(); ++i)
    {
        if (m_programs[i].getStringName() == name)
            return i;
    }
    return static_cast<uint32_t>(-1);
}

RTprogram OxContractWithOxPrograms::nativeOptiXProgramHandle(uint32_t declaration_offset) const
{
    return m_programs[declaration_offset].m_native_optix_program.get();
}

OxProgram OxContractWithOxPrograms::getOxProgramFromDeclarationOffset(uint32_t declaration_offset) const
{
    return m_programs[declaration_offset];
}

size_t OxContractWithOxPrograms::getAttachedProgramsCount() const
{
    return m_programs.size();
}

void OxContractWithOxPrograms::replaceProgramAtOffset(OxProgram const& program, uint32_t declaration_offset)
{
    OxProgram& p = m_programs[declaration_offset];
    if (p.getUniqueIdentifier() != program.getUniqueIdentifier())
        p = program;
}
