#ifndef RA_GEOMETRY_GROUP
#define RA_GEOMETRY_GROUP

#include "optix.h"

#include "ra_fwd.h"
#include "contract_with_context.h"
#include "geometry.h"
#include "util/matrix_types.h"
#include "transformable.h"

#include <cstdint>
#include <memory>
#include <list>

namespace ra {

enum class RaBVHAlgorithm
{
    trbvh, sbvh, bvh, none
};

template<typename T> class RaGeometryGroupAttorney;

class RaGeometryGroup : public RaContractWithRaContext, public RaTransformable, public RaEntity
{
    friend class RaGeometryGroupAttorney<RaSceneSection>;
public:
    RaGeometryGroup(RaContext const& optix_context, RaBVHAlgorithm acceleration_structure_construction_algorithm);

    uint32_t getNumberOfGeometries() const;

    void beginConstruction();
    void addGeometry(RaGeometry const& geometry);
    void endConstruction();

    std::list<RaGeometry> const& geometries() const;

    // required by RaEntity interface
    bool isValid() const override;

    bool isMaterialAssignmentDelayed() const;

private:
    // required by RaTransformable interface
    RTobject getObjectToBeTransformed() const override;

private:
    /*! marks acceleration structure associated with geometry group as "dirty",
     when this is required by at least one of geometries included into the group
     and returns 'true'. If no geometry in the group requires update the function
     updates information regarding the scene entry node and returns 'false'
    */
    bool update(RaObjectHandle top_scene_object) const;

private:
    std::shared_ptr<RTgeometrygroup_api> m_native_geometry_group;
    std::shared_ptr<RTacceleration_api> m_native_acceleration;
    std::list<RaGeometry> m_list_of_geometries;
    bool m_construction_begun;
    bool m_construction_finished;
};

template<> class RaGeometryGroupAttorney<RaSceneSection>
{
    friend class RaSceneSection;

    static RTobject getGeometryGroupNativeHandle(RaGeometryGroup const& parent_geometry_group)
    {
        return parent_geometry_group.isTransformApplied() ?
            static_cast<RTobject>(parent_geometry_group.getNativeOptiXTransformHandle()) :
            static_cast<RTobject>(parent_geometry_group.m_native_geometry_group.get());
    }

    static std::pair<bool, bool> getGeometryGroupConstructionStatus(RaGeometryGroup const& parent_geometry_group)
    {
        return std::make_pair(parent_geometry_group.m_construction_begun, parent_geometry_group.m_construction_finished);
    }

    static bool updateGeometryGroup(RaGeometryGroup const& parent_geometry_group, RaObjectHandle top_scene_object)
    {
        return parent_geometry_group.update(top_scene_object);
    }
};

}

#endif
