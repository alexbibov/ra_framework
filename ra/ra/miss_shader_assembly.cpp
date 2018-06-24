#include "miss_shader_assembly.h"
#include "util/misc.h"

#include <algorithm>

using namespace ra;


RaMissShaderAssembly::RaMissShaderAssembly(std::initializer_list<RaMissShader> init_list):
    RaMissShaderAssembly{ std::vector<RaMissShader>{init_list} }
{
    
}

RaMissShaderAssembly::RaMissShaderAssembly(std::vector<RaMissShader> const& miss_shaders) :
    RaContractWithRaContext{ miss_shaders.begin()->context() }
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

            THROW_RA_ERROR(log_message);
        }

        used_ray_types_mask |= current_mask;
        m_miss_shader_list.insert(ms);
    }
}

util::Optional<RaMissShader> RaMissShaderAssembly::getMissShaderById(RaEntityId const& id) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.getUniqueIdentifier() == id)
            return ms;
    }
    
    return util::Optional<RaMissShader>{};
}

util::Optional<RaMissShader> RaMissShaderAssembly::getMissShaderByName(std::string const& name) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (ms.getStringName() == name)
            return ms;
    }

    return util::Optional<RaMissShader>{};
}

util::Optional<RaMissShader> RaMissShaderAssembly::getMissShaderByRayType(RaRayType ray_type) const
{
    for (auto& ms : m_miss_shader_list)
    {
        if (rayTypeCollectionTo64BitMask(ms.supportedRayTypes()) & (0x1ui64 << static_cast<unsigned>(ray_type)))
            return ms;
    }

    return util::Optional<RaMissShader>{};
}

bool RaMissShaderAssembly::isValid() const
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

void RaMissShaderAssembly::apply(RaObjectHandle top_scene_object) const
{
    for (auto& ms : m_miss_shader_list)
        RaMissShaderAttorney<RaMissShaderAssembly>::applyMissShader(ms, top_scene_object);
}

RaMissShaderAssembly::miss_shader_collection::iterator RaMissShaderAssembly::begin()
{
    return m_miss_shader_list.begin();
}

RaMissShaderAssembly::miss_shader_collection::iterator RaMissShaderAssembly::end()
{
    return m_miss_shader_list.end();
}

RaMissShaderAssembly::miss_shader_collection::const_iterator RaMissShaderAssembly::cbegin()
{
    return m_miss_shader_list.cbegin();
}

RaMissShaderAssembly::miss_shader_collection::const_iterator RaMissShaderAssembly::cend()
{
    return m_miss_shader_list.cend();
}

RaMissShaderAssembly::miss_shader_collection::const_iterator RaMissShaderAssembly::begin() const
{
    return m_miss_shader_list.begin();
}

RaMissShaderAssembly::miss_shader_collection::const_iterator RaMissShaderAssembly::end() const
{
    return m_miss_shader_list.end();
}

uint64_t RaMissShaderAssembly::miss_shader_hasher::operator()(RaMissShader const& ms) const
{
    return rayTypeCollectionTo64BitMask(ms.supportedRayTypes());
}

bool RaMissShaderAssembly::miss_shader_hasher::operator()(RaMissShader const& ms1, RaMissShader const& ms2) const
{
    return ms1.getProgram().getId().native == ms2.getProgram().getId().native;
}
