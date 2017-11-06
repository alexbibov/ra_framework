#include "optix_geometry_group.h"
#include "optix_scene_section.h"
#include "optix_ray_generator.h"


using namespace ox_wrapper;

uint32_t OptiXGeometryGroup::getNumberOfGeometries() const
{
    unsigned int count;
    logOptiXContextError(rtGeometryGroupGetChildCount(m_native_geometry_group.get(), &count));
    return static_cast<uint32_t>(count);
}


OptiXGeometryGroup::OptiXGeometryGroup(OptiXContext const& optix_context, BVHAlgorithm acceleration_structure_construction_algorithm):
    HasContractWithOptiXContext{ optix_context }
{
    RTgeometrygroup geometry_group_native_handle;
    logOptiXContextError(rtGeometryGroupCreate(nativeOptiXContextHandle(), &geometry_group_native_handle));

    m_native_geometry_group.reset(geometry_group_native_handle, 
        [this](RTgeometrygroup gg) -> void
    {
        logOptiXContextError(rtGeometryGroupDestroy(gg));
    });

    RTacceleration acceleration_native;
    logOptiXContextError(rtAccelerationCreate(nativeOptiXContextHandle(), &acceleration_native));
    m_native_acceleration.reset(acceleration_native, 
        [this](RTacceleration a) -> void
    {
        logOptiXContextError(rtAccelerationDestroy(a));
    });

    char const* acceleration_structure_construction_algorithm_name{ nullptr };
    switch (acceleration_structure_construction_algorithm)
    {
    case BVHAlgorithm::trbvh:
        acceleration_structure_construction_algorithm_name = "Trbvh";
        break;
    case BVHAlgorithm::sbvh:
        acceleration_structure_construction_algorithm_name = "Sbvh";
        break;
    case BVHAlgorithm::bvh:
        acceleration_structure_construction_algorithm_name = "Bvh";
        break;
    case BVHAlgorithm::none:
        acceleration_structure_construction_algorithm_name = "NoAccel";
        break;
    }
    logOptiXContextError(rtAccelerationSetBuilder(acceleration_native, acceleration_structure_construction_algorithm_name));
}
