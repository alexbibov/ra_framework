#include "context.h"
#include "material_assembly.h"
#include "contract_with_context.h"

using namespace ox_wrapper;


class OxMaterialAssembly::impl
{
public:
    struct material_comparator { bool operator()(OxMaterial const& m1, OxMaterial const& m2) { m1.rayType() < m2.rayType(); } };

public:
    impl(std::initializer_list<OxMaterial> init_list, OxMaterialAssembly& parent_object)
    {
        unsigned int idx{ 0U };
        for (auto const& e : init_list)
        {
            throwOptiXContextError(rtGeometryInstanceSetMaterial(native_geometry_instance_handle, idx, OxMaterialAttorney<OxMaterialAssembly>::getNativeMaterialHandle(e)));

            if (!m_materials.insert(e).second)
                util::Log::retrieve()->out("Material \"" + e.getStringName() + "\" cannot be added into material assembly (material for ray type \""
                    + std::to_string(e.rayType()) + "\" already exists in the assembly", util::LogMessageType::exclamation);

            ++idx;
        }
    }

    std::set<OxMaterial, material_comparator> const& materials() const { return m_materials; }

private:
    std::set<OxMaterial, material_comparator> m_materials;

};


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
    
    
}

OxMaterialAssembly::~OxMaterialAssembly() = default;

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

bool OxMaterialAssembly::compare_materials::operator()(OxMaterial const& m1, OxMaterial const& m2)
{
    return m1.rayType() < m2.rayType();
}
