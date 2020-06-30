#ifndef RA_MATERIAL_ASSEMBLY_H
#define RA_MATERIAL_ASSEMBLY_H

#include "optix.h"
#include "ra_fwd.h"
#include "material.h"
#include <memory>
#include <unordered_set>
#include <vector>

#include "util/optional.h"

namespace ra {

template<typename T>
class RaMaterialAssemblyAttorney;

class RaMaterialAssembly : public RaContractWithRaContext, public RaEntity
{
    friend class RaMaterialAssemblyAttorney<RaGeometry>;
    friend class RaMaterialAssemblyAttorney<RaGeometryGroup>;

public:
    RaMaterialAssembly(RaContext const& context);
    RaMaterialAssembly(std::initializer_list<RaMaterial> init_list);
    RaMaterialAssembly(std::vector<RaMaterial> const& materials);
    virtual ~RaMaterialAssembly() = default;

    util::Optional<RaMaterial> getMaterialById(RaEntityId const& id) const;
    util::Optional<RaMaterial> getMaterialByName(std::string const& name) const;
    util::Optional<RaMaterial> getMaterialByRayType(RaRayType ray_type) const;
    size_t getMaterialCount() const;
    bool isEmpty() const { return getMaterialCount() == 0; }

    // required by RaEntity interface
    bool isValid() const override;

private:
    struct material_hasher
    {
        uint64_t operator()(RaMaterial const& m) const;
        bool operator()(RaMaterial const& m1, RaMaterial const& m2) const;
    };

    using material_collection = std::unordered_set<RaMaterial, 
        material_hasher, material_hasher>;

private:
    void update(RaObjectHandle top_scene_object) const;
    
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
class RaMaterialAssemblyAttorney<RaGeometry>
{
    friend class RaGeometry;

    static void attachMaterialAssemblyToNativeGeometryHandle(RaMaterialAssembly const& parent_material_assembly, RTgeometry native_geometry_handle)
    {
        if(parent_material_assembly.getMaterialCount())
        {
            THROW_OPTIX_ERROR(parent_material_assembly.nativeOptiXContextHandle(),
                rtGeometryInstanceSetGeometry(parent_material_assembly.m_native_geometry_instance.get(),
                    native_geometry_handle));
        }
    }

    static void updateMaterialAssembly(RaMaterialAssembly const& parent_material_assembly, RaObjectHandle top_scene_object)
    {
        if (parent_material_assembly.getMaterialCount())
            parent_material_assembly.update(top_scene_object);
    }
};

template<>
class RaMaterialAssemblyAttorney<RaGeometryGroup>
{
    friend class RaGeometryGroup;

    static RTgeometryinstance getNativeGeometryInstanceHandle(RaMaterialAssembly const& parent_material_assembly)
    {
        return parent_material_assembly.m_native_geometry_instance.get();
    }

    static bool isDummyMaterialAssembly(RaMaterialAssembly const& parent_material_assembly)
    {
        return parent_material_assembly.m_is_dummy;
    }
};


}

#endif