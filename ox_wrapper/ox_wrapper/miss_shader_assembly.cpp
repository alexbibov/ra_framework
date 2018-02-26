#include "miss_shader_assembly.h"
#include "util/misc.h"

#include <algorithm>

using namespace ox_wrapper;


OxMissShaderAssembly::OxMissShaderAssembly(std::initializer_list<OxMissShader> init_list):
    OxMissShaderAssembly{ std::vector<OxMissShader>{init_list} }
{
    
}

OxMissShaderAssembly::OxMissShaderAssembly(std::vector<OxMissShader> const& miss_shaders) :
    OxContractWithOxContext{ miss_shaders.begin()->context() }
{
    uint64_t used_ray_types_mask{ 0U };
    for (auto& ms : miss_shaders)
    {
        uint64_t current_mask = rayTypeCollectionTo64BitMask(ms.supportedRayTypes());
        uint64_t repeated_uses_mask = current_mask & used_ray_types_mask;
        if (repeated_uses_mask)
        {
            auto repeated_uses_ray_indices = util::misc::getSetBits(repeated_uses_mask);

            std::string log_message = "Unable to insert miss shader \"" + ms.getStringName()
                + "\" into miss shader assembly \"" + getStringName() + "\". Ray types ";

            std::for_each(repeated_uses_ray_indices.begin(), --repeated_uses_ray_indices.end(),
                [&log_message](uint8_t rt_index) {log_message += std::to_string(rt_index) + ","; });
            log_message += std::to_string(repeated_uses_ray_indices[repeated_uses_ray_indices.size() - 1]);

            log_message += " are already in use in this assembly";

            throw OxException{ log_message.c_str(), __FILE__, __FUNCTION__, __LINE__ };
        }

        used_ray_types_mask |= current_mask;
        m_miss_shader_list.insert(ms);
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
        if (rayTypeCollectionTo64BitMask(ms.supportedRayTypes()) & (0x1ui64 << static_cast<unsigned>(ray_type)))
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

void OxMissShaderAssembly::apply(OxObjectHandle top_scene_object) const
{
    for (auto& ms : m_miss_shader_list)
        OxMissShaderAttorney<OxMissShaderAssembly>::applyMissShader(ms, top_scene_object);
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

uint64_t OxMissShaderAssembly::miss_shader_hasher::operator()(OxMissShader const& ms) const
{
    return rayTypeCollectionTo64BitMask(ms.supportedRayTypes());
}

bool OxMissShaderAssembly::miss_shader_hasher::operator()(OxMissShader const& ms1, OxMissShader const& ms2) const
{
    return ms1.getProgram().getId().native == ms2.getProgram().getId().native;
}
