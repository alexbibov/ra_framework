#ifndef OX_WRAPPER_OPTIX_GEOMETRY_H
#define OX_WRAPPER_OPTIX_GEOMETRY_H

#include "ox_wrapper_fwd.h"
#include "optix.h"
#include "hashed_string.h"
#include "optix_material.h"
#include <map>

namespace ox_wrapper {

template<typename T>
class OptiXGeometryAttorney;

class OptiXGeometry
{
    friend class OptiXGeometryAttorney<OptiXGeometryGroup>;

public:
    void attachMaterial(OptiXMaterial const& material, uint16_t slot);
    uint16_t getSlotByMaterialId(uint64_t id) const;


private:
    OptiXGeometry(OptiXGeometryGroup const& optix_geometry_group);

private:
    OptiXGeometryGroup const& m_optix_geometry_group;
    RTgeometry m_native_geometry;
    std::map<uint64_t, std::pair<OptiXMaterial, uint16_t>> m_material_dictionary;
};

template<>
class OptiXGeometryAttorney<OptiXGeometryGroup>
{
    friend class OptiXGeometryGroup;

    OptiXGeometry createOptiXGeometry(OptiXGeometryGroup const& optix_geometry_group)
    {
        return OptiXGeometry{ optix_geometry_group };
    }
};

}

#endif
