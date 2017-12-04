#ifndef OX_WRAPPER_GEOMETRY_GROUP
#define OX_WRAPPER_GEOMETRY_GROUP

#include "optix.h"
#include "fwd.h"
#include "contract_with_context.h"
#include "geometry.h"
#include "util/matrix_types.h"
#include "transformable.h"

#include <cstdint>
#include <memory>
#include <list>

namespace ox_wrapper {

enum class OxBVHAlgorithm
{
    trbvh, sbvh, bvh, none
};

template<typename T> class OxGeometryGroupAttorney;

class OxGeometryGroup : public OxContractWithOxContext, public OxTransformable, public OxEntity
{
    friend class OxGeometryGroupAttorney<OxSceneSection>;
public:
    OxGeometryGroup(OxContext const& optix_context, OxBVHAlgorithm acceleration_structure_construction_algorithm);

    uint32_t getNumberOfGeometries() const;

    void beginConstruction();
    void addGeometry(OxGeometry const& geometry);
    void endConstruction();

    // required by OxEntity interface
    bool isValid() const override;

private:
    // required by OxTransformable interface
    RTobject getTransformedObject() const override;

private:
    /*! marks acceleration structure associated with geometry group as "dirty",
     when this is required by at least one of geometries included into the group
     and returns 'true'. If no geometry in the group requires update the function
     has no effect and returns 'false'
    */
    bool update() const;

private:
    std::shared_ptr<RTgeometrygroup_api> m_native_geometry_group;
    std::shared_ptr<RTacceleration_api> m_native_acceleration;
    std::list<OxGeometry> m_list_of_geometries;
    bool m_construction_begun;
    bool m_construction_finished;
};

template<> class OxGeometryGroupAttorney<OxSceneSection>
{
    friend class OxSceneSection;

    static RTobject getGeometryGroupNativeHandle(OxGeometryGroup const& parent_geometry_group)
    {
        return parent_geometry_group.isTransformApplied() ?
            static_cast<RTobject>(parent_geometry_group.getNativeOptiXTransformHandle()) :
            static_cast<RTobject>(parent_geometry_group.m_native_geometry_group.get());
    }

    static std::pair<bool, bool> getGeometryGroupConstructionStatus(OxGeometryGroup const& parent_geometry_group)
    {
        return std::make_pair(parent_geometry_group.m_construction_begun, parent_geometry_group.m_construction_finished);
    }

    static bool updateGeometryGroup(OxGeometryGroup const& parent_geometry_group)
    {
        return parent_geometry_group.update();
    }
};

}

#endif
