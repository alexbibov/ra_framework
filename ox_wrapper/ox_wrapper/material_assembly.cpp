#include "context.h"
#include "material_assembly.h"
#include "contract_with_context.h"

using namespace ox_wrapper;

OxMaterialAssembly::OxMaterialAssembly(std::initializer_list<OxMaterial> init_list):
    OxContractWithOxContext{ init_list.begin()->context() }
{
    RTgeometryinstance native_geometry_instance_handle;
    throwOptiXContextError(rtGeometryInstanceCreate(nativeOptiXContextHandle(), &native_geometry_instance_handle));
    m_native_geometry_instance.reset(native_geometry_instance_handle,
        [this](RTgeometryinstance gi)->void
    {
        logOptiXContextError(rtGeometryInstanceDestroy(gi));
    });

    throwOptiXContextError(rtGeometryInstanceSetMaterialCount(native_geometry_instance_handle, static_cast<unsigned int>(init_list.size())));
    m_materials.reserve(init_list.size());
    unsigned int idx{ 0U };
    for (auto const& e : init_list)
    {
        throwOptiXContextError(rtGeometryInstanceSetMaterial(native_geometry_instance_handle, idx, OxMaterialAttorney<OxMaterialAssembly>::getNativeMaterialHandle(e)));
        m_materials.push_back(e);
        ++idx;
    }
}

OxMaterial const* OxMaterialAssembly::getMaterialById(OxEntityID const& id) const
{
    for (auto const& e : m_materials)
    {
        if (e.getId() == id)
            return &e;
    }

    return nullptr;
}

OxMaterial const* OxMaterialAssembly::getMaterialByName(std::string const& name) const
{
    for (auto const& e : m_materials)
    {
        if (e.getStringName() == name)
            return &e;
    }

    return nullptr;
}

bool OxMaterialAssembly::isValid() const
{
    RTresult res = rtGeometryInstanceValidate(m_native_geometry_instance.get());
    logOptiXContextError(res);
    
    bool materials_valid{ true };
    for (auto const& e : m_materials)
    {
        if (!e.isValid())
        {
            materials_valid = false;
            break;
        }
    }

    return res == RT_SUCCESS && materials_valid;
}
