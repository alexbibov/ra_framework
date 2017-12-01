#include "context.h"
#include "material.h"

using namespace ox_wrapper;

OxMaterial::OxMaterial(OxProgram const& closest_hit_shader, unsigned int ray_type_index/* = 0U*/):
    OxContractWithOxContext{ closest_hit_shader.context() },
    OxContractWithOxPrograms{ closest_hit_shader },
    m_ray_type_index{ ray_type_index }
{
    RTmaterial native_material_handle;
    throwOptiXContextError(rtMaterialCreate(nativeOptiXContextHandle(), &native_material_handle));

    m_native_material.reset(native_material_handle,
        [this](RTmaterial m) -> void
    {
        logOptiXContextError(rtMaterialDestroy(m));
    });

    throwOptiXContextError(rtMaterialSetClosestHitProgram(native_material_handle, m_ray_type_index, nativeOptiXProgramHandle(0U)));
}

OxMaterial::OxMaterial(OxProgram const& closest_hit_shader, OxProgram const& any_hit_shader, unsigned int ray_type_index/* = 0U*/):
    OxContractWithOxContext{ closest_hit_shader.context() },
    OxContractWithOxPrograms{ closest_hit_shader, any_hit_shader },
    m_ray_type_index{ ray_type_index }
{
    RTmaterial native_material_handle;
    throwOptiXContextError(rtMaterialCreate(nativeOptiXContextHandle(), &native_material_handle));

    m_native_material.reset(native_material_handle,
        [this](RTmaterial m) -> void
    {
        logOptiXContextError(rtMaterialDestroy(m));
    });

    throwOptiXContextError(rtMaterialSetClosestHitProgram(native_material_handle, m_ray_type_index, nativeOptiXProgramHandle(0U)));
    throwOptiXContextError(rtMaterialSetAnyHitProgram(native_material_handle, m_ray_type_index, nativeOptiXProgramHandle(1U)));
}

OxProgram OxMaterial::getClosestHitShader() const
{
    return getOxProgramFromDeclarationOffset(0U);
}

OxProgram OxMaterial::getAnyHitShader() const
{
    return getOxProgramFromDeclarationOffset(1U);
}

unsigned int OxMaterial::rayType() const
{
    return m_ray_type_index;
}

bool OxMaterial::isValid() const
{
    RTresult res = rtMaterialValidate(m_native_material.get());
    logOptiXContextError(res);

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
