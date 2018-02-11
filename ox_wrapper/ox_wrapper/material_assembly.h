#ifndef OX_WRAPPER_MATERIAL_ASSEMBLY_H
#define OX_WRAPPER_MATERIAL_ASSEMBLY_H

#include "optix.h"
#include "fwd.h"
#include "material.h"
#include <memory>
#include <set>
#include <vector>

#include "util/optional.h"

namespace ox_wrapper {

template<typename T>
class OxMaterialAssemblyAttorney;

class OxMaterialAssembly : public OxContractWithOxContext, public OxEntity
{
    friend class OxMaterialAssemblyAttorney<OxGeometry>;
    friend class OxMaterialAssemblyAttorney<OxGeometryGroup>;

public:
    OxMaterialAssembly(OxContext const& context);
    OxMaterialAssembly(std::initializer_list<OxMaterial> init_list);
    OxMaterialAssembly(std::vector<OxMaterial> const& materials);
    virtual ~OxMaterialAssembly() = default;

    util::Optional<OxMaterial> getMaterialById(OxEntityId const& id) const;
    util::Optional<OxMaterial> getMaterialByName(std::string const& name) const;
    util::Optional<OxMaterial> getMaterialByRayType(OxRayType ray_type) const;
    size_t getMaterialCount() const;

    // required by OxEntity interface
    bool isValid() const override;

private:
    struct material_comparator 
    {
        bool operator()(OxMaterial const& m1, OxMaterial const& m2) const;
    };

    using material_collection = std::set<OxMaterial, material_comparator>;

private:
    void update(OxObjectHandle top_scene_object) const;
    
private:
    bool m_is_dummy;
    material_collection m_materials;
    std::shared_ptr<RTgeometryinstance_api> m_native_geometry_instance;

public:
    material_collection::iterator begin();
    material_collection::iterator end();

    material_collection::const_iterator cbegin();
    material_collection::const_iterator cend();

    material_collection::const_iterator begin() const;
    material_collection::const_iterator end() const;
};

template<>
class OxMaterialAssemblyAttorney<OxGeometry>
{
    friend class OxGeometry;

    static void attachMaterialAssemblyToNativeGeometryHandle(OxMaterialAssembly const& parent_material_assembly, RTgeometry native_geometry_handle)
    {
        if(parent_material_assembly.getMaterialCount())
        {
            THROW_OPTIX_ERROR(parent_material_assembly.nativeOptiXContextHandle(),
                rtGeometryInstanceSetGeometry(parent_material_assembly.m_native_geometry_instance.get(),
                    native_geometry_handle));
        }
    }

    static void updateMaterialAssembly(OxMaterialAssembly const& parent_material_assembly, OxObjectHandle top_scene_object)
    {
        if (parent_material_assembly.getMaterialCount())
            parent_material_assembly.update(top_scene_object);
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

    static bool isDummyMaterialAssembly(OxMaterialAssembly const& parent_material_assembly)
    {
        return parent_material_assembly.m_is_dummy;
    }
};


}

#endif