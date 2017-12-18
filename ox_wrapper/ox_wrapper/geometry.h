#ifndef OX_WRAPPER_GEOMETRY_H
#define OX_WRAPPER_GEOMETRY_H

#include "optix.h"
#include "fwd.h"
#include "entity.h"
#include "material_assembly.h"
#include "util/optional.h"

#include <map>
#include <list>

namespace ox_wrapper {

template<typename T> class OxGeometryAttorney;

class OxGeometry : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxGeometryAttorney<OxGeometryGroup>;

public:
    OxGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader);

    OxGeometry(OxProgram const& intersection_shader, 
        OxProgram const& aabb_shader, OxMaterialAssembly const& material_assembly);

    util::Optional<OxMaterialAssembly> getMaterialAssembly() const;
    void setMaterialAssembly(OxMaterialAssembly const& material_assembly);

    OxProgram getAABBShader() const;
    OxProgram getIntersectionShader() const;
    unsigned int getPrimitiveCount() const;

    // required by OxEntity interface
    bool isValid() const override;

protected:
    void setPrimitiveCount(unsigned int num_primitives);
    void markDirty();

private:
    void update(OxObjectHandle top_scene_object) const;

private:
    std::shared_ptr<std::pair<RTgeometry, bool>> m_native_geometry;
    util::Optional<OxMaterialAssembly> m_material_assembly;
};

template<> 
class OxGeometryAttorney<OxGeometryGroup>
{
    friend class OxGeometryGroup;

    static bool isGeometryDirty(OxGeometry const& parent_geometry)
    {
        return parent_geometry.m_native_geometry->second;
    }

    static void markGeometryClean(OxGeometry const& parent_geometry)
    {
        parent_geometry.m_native_geometry->second = false;
    }

    static void updateGeometry(OxGeometry const& parent_geometry, OxObjectHandle top_scene_object)
    {
        parent_geometry.update(top_scene_object);
    }
};

}

#endif