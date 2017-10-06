#include "optix_scene_section.h"
#include "optix_geometry_group.h"

using namespace ox_wrapper;

OptiXRayGenerator const& OptiXSceneSection::getOptiXRayGenerator() const
{
    return m_optix_ray_generator;
}

OptiXGeometryGroup OptiXSceneSection::createOptiXGeometryGroup() const
{
    // TODO: insert return statement here
}

void OptiXSceneSection::runRayTracing() const
{
}
