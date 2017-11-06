#include "optix_scene_section.h"
#include "optix_geometry_group.h"
#include "optix_ray_generator.h"

using namespace ox_wrapper;

OptiXSceneSection::OptiXSceneSection(OptiXRayGenerator const & optix_ray_generator):
    m_optix_ray_generator{ optix_ray_generator }
{
}

OptiXRayGenerator const& OptiXSceneSection::rayGenerator() const
{
    return m_optix_ray_generator;
}

void OptiXSceneSection::runRayTracing() const
{
    OptiXRayGeneratorAttorney<OptiXSceneSection>::launchRayGenerator(m_optix_ray_generator);
}
