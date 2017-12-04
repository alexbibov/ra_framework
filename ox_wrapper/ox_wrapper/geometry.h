#ifndef OX_WRAPPER_GEOMETRY_H
#define OX_WRAPPER_GEOMETRY_H

#include "optix.h"
#include "fwd.h"
#include "entity.h"
#include "material_assembly.h"

#include <map>
#include <list>

namespace ox_wrapper {

template<typename T> class OxGeometryAttorney;

class OxGeometry : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
    friend class OxGeometryAttorney<OxGeometryGroup>;

public:
    OxGeometry(OxProgram const& intersection_shader, 
        OxProgram const& aabb_shader, OxMaterialAssembly const& material_assembly);

    OxMaterialAssembly getMaterialAssembly() const;

    OxProgram getAABBShader() const;
    OxProgram getIntersectionShader() const;
    unsigned int getPrimitiveCount() const;

    // required by OxEntity interface
    bool isValid() const override;

protected:
    void setPrimitiveCount(unsigned int num_primitives);
    void markDirty();

private:
    std::shared_ptr<std::pair<RTgeometry, bool>> m_native_geometry;
    OxMaterialAssembly const m_material_assembly;
};

template<> 
class OxGeometryAttorney<OxGeometryGroup>
{
    friend class OxGeometryGroup;

    static bool isGeometryDirty(OxGeometry const& parent_geometry)
    {
        return parent_geometry.m_native_geometry->second;
    }

    static void markGeometryClean(OxGeometry& parent_geometry)
    {
        parent_geometry.m_native_geometry->second = false;
    }
};

}

#endif
