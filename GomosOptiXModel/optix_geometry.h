#ifndef OX_WRAPPER_OPTIX_GEOMETRY_H
#define OX_WRAPPER_OPTIX_GEOMETRY_H

#include "optix.h"
#include "ox_wrapper_fwd.h"

#include "hashed_string.h"
#include "optix_material.h"

#include <map>

namespace ox_wrapper {

class OptiXGeometry
{
public:
    OptiXGeometry();

    void attachMaterial(OptiXMaterial const& material, uint16_t slot);
    uint16_t getSlotByMaterialId(uint64_t id) const;

    OptiXGeometryGroup const& geometryGroup() const;

private:
    std::shared_ptr<RTgeometry_api> m_native_geometry;
    std::map<uint64_t, std::pair<OptiXMaterial, uint16_t>> m_material_dictionary;
};

}

#endif
