#include "context.h"
#include "material.h"

using namespace ra;

RaMaterial::RaMaterial(
    util::Optional<RaProgram> const& closest_hit_shader, 
    util::Optional<RaProgram> const& any_hit_shader, 
    RaRayTypeCollection const& supported_ray_types/* = RayTypeCollection{ 1U, RaRayType::unknown }*/)
    : RaContractWithRaContext{ 
        closest_hit_shader.isValid() ? 
        static_cast<RaProgram const&>(closest_hit_shader).context()
        : static_cast<RaProgram const&>(any_hit_shader).context() 
     }
    , RaContractWithRaPrograms{ 
        closest_hit_shader.isValid() && any_hit_shader.isValid() ?
        std::initializer_list<RaProgram>{
            static_cast<RaProgram const&>(closest_hit_shader),
            static_cast<RaProgram const&>(any_hit_shader) }
        : closest_hit_shader.isValid() ? 
            std::initializer_list<RaProgram>{ static_cast<RaProgram const&>(closest_hit_shader) }
            : std::initializer_list<RaProgram>{ static_cast<RaProgram const&>(any_hit_shader)} }
    , m_supported_ray_types{ supported_ray_types }
    , m_closest_hit_program_offset{ closest_hit_shader.isValid() ? 0 : -1 }
    , m_any_hit_program_offset{ 
                closest_hit_shader.isValid() && any_hit_shader.isValid() ? 
                1 
                : any_hit_shader.isValid() ? 0 : -1 }
{
    RTmaterial native_material_handle;
    THROW_OPTIX_ERROR(
        nativeOptiXContextHandle(),
        rtMaterialCreate(nativeOptiXContextHandle(), &native_material_handle)
    );

    m_native_material.reset(native_material_handle,
        [this](RTmaterial m) -> void
    {
        LOG_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtMaterialDestroy(m)
        );
    });

    if(closest_hit_shader.isValid())
    {
        for(auto ray_type : m_supported_ray_types)
        {
            THROW_OPTIX_ERROR(
                nativeOptiXContextHandle(),
                rtMaterialSetClosestHitProgram(
                    native_material_handle,
                    static_cast<unsigned int>(ray_type),
                    nativeOptiXProgramHandle(m_closest_hit_program_offset))
            );
        }
    }

    if(any_hit_shader.isValid())
    {
        for(auto ray_type : m_supported_ray_types)
        {
            THROW_OPTIX_ERROR(
                nativeOptiXContextHandle(),
                rtMaterialSetAnyHitProgram(
                    native_material_handle,
                    static_cast<unsigned int>(ray_type),
                    nativeOptiXProgramHandle(m_any_hit_program_offset))
            );
        }
    }
}

util::Optional<RaProgram> RaMaterial::getClosestHitShader() const
{
    if (m_closest_hit_program_offset >= 0) 
    {
        return getRaProgramFromDeclarationOffset(m_closest_hit_program_offset);
    }
    else
    {
        return util::Optional<RaProgram>{};
    }
}

util::Optional<RaProgram> RaMaterial::getAnyHitShader() const
{
    if (m_any_hit_program_offset >= 0)
    {
        return util::Optional<RaProgram>{getRaProgramFromDeclarationOffset(m_any_hit_program_offset)};
    }
    else
    {
        return util::Optional<RaProgram>{};
    }
}

RaRayTypeCollection RaMaterial::supportedRayTypes() const
{
    return m_supported_ray_types;
}

bool ra::RaMaterial::supportsRayType(RaRayType ray_type) const
{
    return std::find(m_supported_ray_types.begin(), m_supported_ray_types.end(), ray_type) 
        != m_supported_ray_types.end();
}

bool RaMaterial::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtMaterialValidate(m_native_material.get()));

    bool programs_valid{ true };
    for (size_t i = 0U; i < getAttachedProgramsCount(); ++i)
    {
        if (!getRaProgramFromDeclarationOffset(static_cast<uint32_t>(i)).isValid())
        {
            programs_valid = false;
            break;
        }
    }

    return res == RT_SUCCESS && programs_valid;
}

void RaMaterial::update(RaObjectHandle top_scene_object) const
{
    if (m_closest_hit_program_offset >= 0)
        getRaProgramFromDeclarationOffset(m_closest_hit_program_offset).setVariableValue("ra_entry_node", top_scene_object);

    if (m_any_hit_program_offset >= 0)
        getRaProgramFromDeclarationOffset(m_any_hit_program_offset).setVariableValue("ra_entry_node", top_scene_object);
}
