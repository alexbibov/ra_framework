#ifndef OX_WRAPPER_OPTIX_GEOMETRY_GROUP
#define OX_WRAPPER_OPTIX_GEOMETRY_GROUP

#include "optix.h"
#include "ox_wrapper_fwd.h"
#include "has_contract_with_optix_context.h"
#include "optix_geometry.h"
#include "matrix_types.h"

#include <cstdint>
#include <memory>
#include <list>

namespace ox_wrapper {

enum class BVHAlgorithm
{
    trbvh, sbvh, bvh, none
};

class OptiXGeometryGroup : public HasContractWithOptiXContext
{
public:
    OptiXGeometryGroup(OptiXContext const& optix_context, BVHAlgorithm acceleration_structure_construction_algorithm);
    virtual ~OptiXGeometryGroup() = default;

    uint32_t getNumberOfGeometries() const;

    void beginTransformationGroup(OptiXTransform const& transform);
    void endTransformationGroup();

    void addGeometry(OptiXGeometry const& geometry);

private:
    std::shared_ptr<RTgeometrygroup_api> m_native_geometry_group;
    std::shared_ptr<RTacceleration_api> m_native_acceleration;

    std::list<OptiXGeometry> m_list_of_geometries;
};

}

#endif
