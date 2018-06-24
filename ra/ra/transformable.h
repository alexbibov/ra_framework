#ifndef OX_WRAPPER_TRANSFORMABLE_H
#define OX_WRAPPER_TRANSFORMABLE_H

#include "transform.h"

namespace ra {

class OxTransformable
{
public:
    OxTransformable(OxContext const& optix_context);

    void applyTransform(OxTransform const& transform);
    OxTransform getTransform() const;

protected:
    bool isTransformApplied() const;
    RTtransform getNativeOptiXTransformHandle() const;

private:
    virtual RTobject getObjectToBeTransformed() const = 0;

private:
    OxTransform m_transform;
    bool m_is_transform_applied;
};

}

#endif
