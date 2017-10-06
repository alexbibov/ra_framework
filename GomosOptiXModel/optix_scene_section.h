#ifndef OX_WRAPPER_OPTIX_SCENE_SECTION_H
#define OX_WRAPPER_OPTIX_SCENE_SECTION_H

#include "ox_wrapper_fwd.h"
#include "optix.h"
#include <vector>

namespace ox_wrapper {
    template<typename T>
    class OptiXSceneSectionAttorney;

    class OptiXSceneSection
    {
        friend class OptiXSceneSectionAttorney<OptiXScene>;
    public:
        OptiXRayGenerator const& getOptiXRayGenerator() const;
        OptiXGeometryGroup createOptiXGeometryGroup() const;

    private:
        OptiXSceneSection(OptiXRayGenerator const& optix_ray_generator);
        void runRayTracing() const;

    private:
        OptiXRayGenerator const& m_optix_ray_generator;
        std::vector<RTgeometrygroup> m_native_geometry_groups;
    };


    template<>
    class OptiXSceneSectionAttorney<OptiXScene>
    {
        friend class OptiXScene;

        static OptiXSceneSection createOptixSceneSection(OptiXRayGenerator const& optix_ray_generator)
        {
            return OptiXSceneSection{ optix_ray_generator };
        }

        static void runRayTracing(OptiXSceneSection const& parent)
        {
            parent.runRayTracing();
        }
    };

};


#endif
