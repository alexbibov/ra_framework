#ifndef OX_WRAPPER_TRANSFORMABLE_H
#define OX_WRAPPER_TRANSFORMABLE_H

#include "transform.h"

namespace ox_wrapper {

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
    virtual RTobject getTransformedObject() const = 0;

private:
    OxTransform m_transform;
    bool m_is_transform_applied;
};

}

#endif
