#ifndef OX_WRAPPER_SCENE_SECTION_H
#define OX_WRAPPER_SCENE_SECTION_H

#include "optix.h"
#include "ra/ra_fwd.h"
#include "ra/entity.h"
#include "ra/geometry_group.h"

#include <list>

namespace ra {

    class OxSceneSection final : public OxContractWithOxContext, public OxEntity, public OxTransformable
    {
    public:
        OxSceneSection(OxContext const& context, OxBVHAlgorithm acceleration_structure_construction_algorithm);

        void beginConstruction();
        void addGeometryGroup(OxGeometryGroup const& geometry_group);
        void addSceneSection(OxSceneSection const& scene_section);
        void endConstruction();

        std::list<OxSceneSection> const& sceneSections() const;
        std::list<OxGeometryGroup> const& geometryGroups() const;

        // required by OxEntity interface
        bool isValid() const override;

        void trace(OxRayGenerator const& ray_caster) const;

    private:
        RTobject getEntryNode() const;
        bool _update(OxRayGenerator const& ray_caster, OxObjectHandle top_scene_object) const;
        void _trace(OxRayGenerator const& ray_caster) const;

        // required by OxTransformable interface
        RTobject getObjectToBeTransformed() const override;

    private:
        std::shared_ptr<RTgroup_api> m_native_group_handle;
        std::shared_ptr<RTacceleration_api> m_native_acceleration_handle;
        std::list<OxGeometryGroup> m_geometry_groups;
        std::list<OxSceneSection> m_attached_scene_sections;
        bool m_construction_begun;
        bool m_construction_finished;
    };

};


#endif
