#ifndef OX_WRAPPER_OPTIX_SCENE_SECTION_H
#define OX_WRAPPER_OPTIX_SCENE_SECTION_H

#include "optix.h"
#include "ox_wrapper_fwd.h"

#include <list>

namespace ox_wrapper {

    template<typename T> class OptiXSceneSectionAttorney;

    class OptiXSceneSection final
    {
        friend class OptiXSceneSectionAttorney<OptiXScene>;

    public:
        OptiXSceneSection(OptiXRayGenerator const& optix_ray_generator);

        OptiXRayGenerator const& rayGenerator() const;

        void addGeometryGroup(OptiXGeometryGroup const& optix_geometry_group);

    private:
        void runRayTracing() const;

    private:
        OptiXRayGenerator const& m_optix_ray_generator;
        std::list<OptiXGeometryGroup> m_geometry_groups;
    };

    template<> class OptiXSceneSectionAttorney<OptiXScene>
    {
        friend class OptiXScene;
        
        static void runRayTracingForSceneSection(OptiXSceneSection const& optix_scene_section)
        {
            optix_scene_section.runRayTracing();
        }
    };

};


#endif
