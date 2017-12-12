#include "miss_shader_assembly.h"

using namespace ox_wrapper;


OxMissShaderAssembly::OxMissShaderAssembly(std::initializer_list<OxMissShader> init_list):
    OxContractWithOxContext{ init_list.begin()->context() }
{
    m_miss_shader_list.reserve(init_list.size());

    for (auto& ms : init_list)
    {
        m_miss_shader_list.push_back(ms);
    }
}

OxMissShader const* OxMissShaderAssembly::getMissShaderById(OxEntityID const& id) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.getId() == id)
            return &ms;
    }
    
    return nullptr;
}

OxMissShader const* OxMissShaderAssembly::getMissShaderByName(std::string const& name) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.getStringName() == name)
            return &ms;
    }

    return nullptr;
}

bool OxMissShaderAssembly::isValid() const
{
    for (auto& ms : m_miss_shader_list)
        if (!ms.isValid()) return false;

    return true;
}

void OxMissShaderAssembly::apply() const
{
    for (auto& ms : m_miss_shader_list)
        OxMissShaderAttorney<OxMissShaderAssembly>::applyMissShader(ms);
}
