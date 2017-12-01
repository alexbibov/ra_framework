#ifndef OX_WRAPPER_GEOMETRY_H
#define OX_WRAPPER_GEOMETRY_H

#include "optix.h"
#include "fwd.h"
#include "entity.h"
#include "material_assembly.h"

#include <map>
#include <list>

namespace ox_wrapper {

class OxGeometry : public OxContractWithOxContext, public OxContractWithOxPrograms, public OxEntity
{
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

private:
    std::shared_ptr<RTgeometry_api> m_native_geometry;
    OxMaterialAssembly const m_material_assembly;
};

}

#endif
