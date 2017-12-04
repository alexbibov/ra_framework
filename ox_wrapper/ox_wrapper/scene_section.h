#ifndef OX_WRAPPER_SCENE_SECTION_H
#define OX_WRAPPER_SCENE_SECTION_H

#include "optix.h"
#include "fwd.h"
#include "entity.h"
#include "geometry_group.h"

#include <list>

namespace ox_wrapper {

    template<typename T> class OxSceneSectionAttorney;

    class OxSceneSection final : public OxContractWithOxContext, public OxEntity, public OxTransformable
    {
        friend class OxSceneSectionAttorney<OxScene>;

    public:
        OxSceneSection(OxRayGenerator const& optix_ray_generator, OxBVHAlgorithm acceleration_structure_construction_algorithm);

        OxRayGenerator const& rayGenerator() const;

        void beginConstruction();
        void addGeometryGroup(OxGeometryGroup const& geometry_group);
        void addSceneSection(OxSceneSection const& scene_section);
        void endConstruction();

        //! joins this scene section with another scene section

        // required by OxEntity interface
        bool isValid() const override;

    private:
        RTobject getEntryNode() const;
        bool update() const;
        void runRayTracing() const;

        // required by OxTransformable interface
        RTobject getTransformedObject() const override;

    private:
        OxRayGenerator const& m_optix_ray_generator;
        std::shared_ptr<RTgroup_api> m_native_group_handle;
        std::shared_ptr<RTacceleration_api> m_native_acceleration_handle;
        std::list<OxGeometryGroup> m_geometry_groups;
        std::list<OxSceneSection> m_attached_scene_sections;
        bool m_construction_begun;
        bool m_construction_finished;
    };

    template<> class OxSceneSectionAttorney<OxScene>
    {
        friend class OxScene;
        
        static void runRayTracingForSceneSection(OxSceneSection const& parent_scene_section)
        {
            parent_scene_section.update();
            parent_scene_section.runRayTracing();
        }
    };

};


#endif
