#ifndef RA_GEOMETRY_H
#define RA_GEOMETRY_H

#include "optix.h"

#include "ra_fwd.h"
#include "entity.h"
#include "material_assembly.h"
#include "util/optional.h"

#include <map>
#include <list>

namespace ra {

template<typename T> class RaGeometryAttorney;

class RaGeometry : public RaContractWithRaContext, public RaContractWithRaPrograms, public RaEntity
{
    friend class RaGeometryAttorney<RaGeometryGroup>;

public:
    RaGeometry(RaProgram const& intersection_shader, RaProgram const& aabb_shader);

    RaGeometry(RaProgram const& intersection_shader, 
        RaProgram const& aabb_shader, RaMaterialAssembly const& material_assembly);

    util::Optional<RaMaterialAssembly> getMaterialAssembly() const;
    void setMaterialAssembly(RaMaterialAssembly const& material_assembly) const;

    RaProgram getAABBShader() const;
    RaProgram getIntersectionShader() const;
    unsigned int getPrimitiveCount() const;

    // required by RaEntity interface
    bool isValid() const override;

    //! Checks if geometry has valid material
    bool hasMaterial() const { return m_geometry_blueprint->material_assembly.isValid(); }

protected:
    void setPrimitiveCount(unsigned int num_primitives);
    void markDirty();

private:
    void update(RaObjectHandle top_scene_object) const;

private:
    struct geometry_blueprint 
    {
        RTgeometry native_handle;
        bool is_dirty;
        util::Optional<RaMaterialAssembly> material_assembly;
    };

private:
    std::shared_ptr<geometry_blueprint> m_geometry_blueprint;
};

template<> 
class RaGeometryAttorney<RaGeometryGroup>
{
    friend class RaGeometryGroup;

    static bool isGeometryDirty(RaGeometry const& parent_geometry)
    {
        return parent_geometry.m_geometry_blueprint->is_dirty;
    }

    static void markGeometryClean(RaGeometry const& parent_geometry)
    {
        parent_geometry.m_geometry_blueprint->is_dirty = false;
    }

    static void updateGeometry(RaGeometry const& parent_geometry, RaObjectHandle top_scene_object)
    {
        parent_geometry.update(top_scene_object);
    }
};

}

#endif
