#ifndef OX_WRAPPER_OPTIX_GEOMETRY_GROUP
#define OX_WRAPPER_OPTIX_GEOMETRY_GROUP

#include "ox_wrapper_fwd.h"
#include "optix.h"

#include <cstdint>

namespace ox_wrapper {

class OptiXGeometryGroup
{
public:
    uint32_t getNumberOfGeometries() const;
    OptiXGeometry createGeometry();

    void beginTransformationGroup(OptiXTransform const& transform);
    void endTransformationGroup();

private:
    OptiXGeometryGroup(OptiXSceneSection const& optix_scene_section);

private:
    OptiXSceneSection const& m_optix_scene_section;
    RTgeometrygroup m_native_geometry_group;
};

}

#endif
