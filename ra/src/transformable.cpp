#include "transformable.h"

using namespace ra;

RaTransformable::RaTransformable(RaContext const& optix_context):
    m_transform{ optix_context },
    m_is_transform_applied{ false }
{

}

void RaTransformable::applyTransform(RaTransform const& transform)
{
    RaTransformAttorney<RaTransformable>::setTransformedObject(transform, getObjectToBeTransformed());
    m_is_transform_applied = true;
}

RaTransform RaTransformable::getTransform() const
{
    return m_transform;
}

bool RaTransformable::isTransformApplied() const
{
    return m_is_transform_applied;
}

RTtransform RaTransformable::getNativeOptiXTransformHandle() const
{
    return RaTransformAttorney<RaTransformable>::getNativeOptiXTransformHandle(m_transform);
}
