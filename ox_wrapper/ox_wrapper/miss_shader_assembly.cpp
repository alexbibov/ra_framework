#include "miss_shader_assembly.h"

using namespace ox_wrapper;


OxMissShaderAssembly::OxMissShaderAssembly(std::initializer_list<OxMissShader> init_list):
    OxContractWithOxContext{ init_list.begin()->context() }
{
    for (auto& ms : init_list)
    {
        if (!m_miss_shader_list.insert(ms).second)
            util::Log::retrieve()->out("WARNING: unable to insert miss shader \"" + ms.getStringName() +
                "\" into miss shader assembly (miss shader for ray type \"" + std::to_string(static_cast<unsigned int>(ms.rayType()))
                + "\" already exists in this assembly)", util::LogMessageType::exclamation);
    }
}

util::Optional<OxMissShader> OxMissShaderAssembly::getMissShaderById(OxEntityId const& id) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.getUniqueIdentifier() == id)
            return ms;
    }
    
    return util::Optional<OxMissShader>{};
}

util::Optional<OxMissShader> OxMissShaderAssembly::getMissShaderByName(std::string const& name) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.getStringName() == name)
            return ms;
    }

    return util::Optional<OxMissShader>{};
}

util::Optional<OxMissShader> OxMissShaderAssembly::getMissShaderByRayType(OxRayType ray_type) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.rayType() == ray_type)
            return ms;
    }

    return util::Optional<OxMissShader>{};
}

bool OxMissShaderAssembly::isValid() const
{
    if (!m_miss_shader_list.size())
    {
        util::Log::retrieve()->out("WARNING: miss shader assembly \"" + getStringName() + "\" is empty", util::LogMessageType::exclamation);
        return false;
    }

    for (auto& ms : m_miss_shader_list)
        if (!ms.isValid()) return false;

    return true;
}

void OxMissShaderAssembly::apply() const
{
    for (auto& ms : m_miss_shader_list)
        OxMissShaderAttorney<OxMissShaderAssembly>::applyMissShader(ms);
}

OxMissShaderAssembly::miss_shader_collection::iterator OxMissShaderAssembly::begin()
{
    return m_miss_shader_list.begin();
}

OxMissShaderAssembly::miss_shader_collection::iterator OxMissShaderAssembly::end()
{
    return m_miss_shader_list.end();
}

OxMissShaderAssembly::miss_shader_collection::const_iterator OxMissShaderAssembly::cbegin()
{
    return m_miss_shader_list.cbegin();
}

OxMissShaderAssembly::miss_shader_collection::const_iterator OxMissShaderAssembly::cend()
{
    return m_miss_shader_list.cend();
}

OxMissShaderAssembly::miss_shader_collection::const_iterator OxMissShaderAssembly::begin() const
{
    return m_miss_shader_list.begin();
}

OxMissShaderAssembly::miss_shader_collection::const_iterator OxMissShaderAssembly::end() const
{
    return m_miss_shader_list.end();
}

bool OxMissShaderAssembly::miss_shader_comparator::operator()(OxMissShader const& ms1, OxMissShader const& ms2) const
{
    return ms1.rayType() < ms2.rayType();
}
