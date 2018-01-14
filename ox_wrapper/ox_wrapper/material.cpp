#include "context.h"
#include "material.h"

using namespace ox_wrapper;

OxMaterial::OxMaterial(util::Optional<OxProgram> const& closest_hit_shader, util::Optional<OxProgram> const& any_hit_shader, OxRayType ray_type):
    OxContractWithOxContext{ closest_hit_shader.isValid() ? static_cast<OxProgram const&>(closest_hit_shader).context() :
                             static_cast<OxProgram const&>(any_hit_shader).context() },
    OxContractWithOxPrograms{ closest_hit_shader.isValid() && any_hit_shader.isValid() ?
    std::initializer_list<OxProgram>{static_cast<OxProgram const&>(closest_hit_shader), static_cast<OxProgram const&>(any_hit_shader)} :
    closest_hit_shader.isValid() ? std::initializer_list<OxProgram>{static_cast<OxProgram const&>(closest_hit_shader)} :
    std::initializer_list<OxProgram>{static_cast<OxProgram const&>(any_hit_shader)} },
    m_ray_type{ ray_type },
    m_closest_hit_program_offset{ closest_hit_shader.isValid() ? 0 : -1 },
    m_any_hit_program_offset{ closest_hit_shader.isValid() && any_hit_shader.isValid() ? 1 : any_hit_shader.isValid() ? 0 : -1 }
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
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtMaterialSetClosestHitProgram(native_material_handle, static_cast<unsigned int>(m_ray_type), nativeOptiXProgramHandle(m_closest_hit_program_offset))
        );
    }

    if(any_hit_shader.isValid())
    {
        THROW_OPTIX_ERROR(
            nativeOptiXContextHandle(),
            rtMaterialSetAnyHitProgram(native_material_handle, static_cast<unsigned int>(m_ray_type), nativeOptiXProgramHandle(m_any_hit_program_offset))
        );
    }
}

util::Optional<OxProgram> OxMaterial::getClosestHitShader() const
{
    if (m_closest_hit_program_offset >= 0) 
    {
        return getOxProgramFromDeclarationOffset(m_closest_hit_program_offset);
    }
    else
    {
        return util::Optional<OxProgram>{};
    }
}

util::Optional<OxProgram> OxMaterial::getAnyHitShader() const
{
    if (m_any_hit_program_offset >= 0)
    {
        return util::Optional<OxProgram>{getOxProgramFromDeclarationOffset(m_any_hit_program_offset)};
    }
    else
    {
        return util::Optional<OxProgram>{};
    }
}

OxRayType OxMaterial::rayType() const
{
    return m_ray_type;
}

bool OxMaterial::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtMaterialValidate(m_native_material.get()));

    bool programs_valid{ true };
    for (size_t i = 0U; i < getAttachedProgramsCount(); ++i)
    {
        if (!getOxProgramFromDeclarationOffset(static_cast<uint32_t>(i)).isValid())
        {
            programs_valid = false;
            break;
        }
    }

    return res == RT_SUCCESS && programs_valid;
}

void OxMaterial::update(OxObjectHandle top_scene_object) const
{
    if (m_closest_hit_program_offset >= 0)
        getOxProgramFromDeclarationOffset(m_closest_hit_program_offset).setVariableValue("ox_entry_node", top_scene_object);

    if (m_any_hit_program_offset >= 0)
        getOxProgramFromDeclarationOffset(m_any_hit_program_offset).setVariableValue("ox_entry_node", top_scene_object);
}
