#ifndef OX_WRAPPER_MATERIAL_ASSEMBLY_H
#define OX_WRAPPER_MATERIAL_ASSEMBLY_H

#include "optix.h"
#include "fwd.h"
#include "material.h"
#include <memory>
#include <vector>


namespace ox_wrapper {

template<typename T>
class OxMaterialAssemblyAttorney;

class OxMaterialAssembly : public OxContractWithOxContext, public OxEntity
{
    friend class OxMaterialAssemblyAttorney<OxGeometry>;
    friend class OxMaterialAssemblyAttorney<OxGeometryGroup>;

public:
    OxMaterialAssembly(std::initializer_list<OxMaterial> init_list);

    OxMaterial const* getMaterialById(OxEntityID const& id) const;
    OxMaterial const* getMaterialByName(std::string const& name) const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    std::vector<OxMaterial> m_materials;
    std::shared_ptr<RTgeometryinstance_api> m_native_geometry_instance;
};

template<>
class OxMaterialAssemblyAttorney<OxGeometry>
{
    friend class OxGeometry;

    static void attachMaterialAssemblyToNativeGeometryHandle(OxMaterialAssembly const& parent_material_assembly, RTgeometry native_geometry_handle)
    {
        parent_material_assembly.throwOptiXContextError(
            rtGeometryInstanceSetGeometry(parent_material_assembly.m_native_geometry_instance.get(),
            native_geometry_handle));
    }
};

template<>
class OxMaterialAssemblyAttorney<OxGeometryGroup>
{
    friend class OxGeometryGroup;

    static RTgeometryinstance getNativeGeometryInstanceHandle(OxMaterialAssembly const& parent_material_assembly)
    {
        return parent_material_assembly.m_native_geometry_instance.get();
    }
};


}

#endif