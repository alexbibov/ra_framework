#ifndef RA_TRANSFORMABLE_H
#define RA_TRANSFORMABLE_H

#include "transform.h"

namespace ra {

class RaTransformable
{
public:
    RaTransformable(RaContext const& optix_context);

    void applyTransform(RaTransform const& transform);
    RaTransform getTransform() const;

protected:
    bool isTransformApplied() const;
    RTtransform getNativeOptiXTransformHandle() const;

private:
    virtual RTobject getObjectToBeTransformed() const = 0;

private:
    RaTransform m_transform;
    bool m_is_transform_applied;
};

}

#endif
