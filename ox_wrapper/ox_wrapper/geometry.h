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
    void setMaterialAssembly(OxMaterialAssembly const& material_assembly) const;

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
    struct geometry_blueprint 
    {
        RTgeometry native_handle;
        bool is_dirty;
        util::Optional<OxMaterialAssembly> material_assembly;
    };

private:
    std::shared_ptr<geometry_blueprint> m_geometry_blueprint;
};

template<> 
class OxGeometryAttorney<OxGeometryGroup>
{
    friend class OxGeometryGroup;

    static bool isGeometryDirty(OxGeometry const& parent_geometry)
    {
        return parent_geometry.m_geometry_blueprint->is_dirty;
    }

    static void markGeometryClean(OxGeometry const& parent_geometry)
    {
        parent_geometry.m_geometry_blueprint->is_dirty = false;
    }

    static void updateGeometry(OxGeometry const& parent_geometry, OxObjectHandle top_scene_object)
    {
        parent_geometry.update(top_scene_object);
    }
};

}

#endif
