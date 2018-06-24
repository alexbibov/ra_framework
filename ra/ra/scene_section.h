#ifndef RA_SCENE_SECTION_H
#define RA_SCENE_SECTION_H

#include "optix.h"
#include "ra/ra_fwd.h"
#include "ra/entity.h"
#include "ra/geometry_group.h"

#include <list>

namespace ra {

    class RaSceneSection final : public RaContractWithRaContext, public RaEntity, public RaTransformable
    {
    public:
        RaSceneSection(RaContext const& context, RaBVHAlgorithm acceleration_structure_construction_algorithm);

        void beginConstruction();
        void addGeometryGroup(RaGeometryGroup const& geometry_group);
        void addSceneSection(RaSceneSection const& scene_section);
        void endConstruction();

        std::list<RaSceneSection> const& sceneSections() const;
        std::list<RaGeometryGroup> const& geometryGroups() const;

        // required by RaEntity interface
        bool isValid() const override;

        void trace(RaRayGenerator const& ray_caster) const;

    private:
        RTobject getEntryNode() const;
        bool _update(RaRayGenerator const& ray_caster, RaObjectHandle top_scene_object) const;
        void _trace(RaRayGenerator const& ray_caster) const;

        // required by RaTransformable interface
        RTobject getObjectToBeTransformed() const override;

    private:
        std::shared_ptr<RTgroup_api> m_native_group_handle;
        std::shared_ptr<RTacceleration_api> m_native_acceleration_handle;
        std::list<RaGeometryGroup> m_geometry_groups;
        std::list<RaSceneSection> m_attached_scene_sections;
        bool m_construction_begun;
        bool m_construction_finished;
    };

};


#endif
