#include "transformable.h"

using namespace ox_wrapper;

OxTransformable::OxTransformable(OxContext const& optix_context):
    m_transform{ optix_context },
    m_is_transform_applied{ false }
{

}

void OxTransformable::applyTransform(OxTransform const& transform)
{
    OxTransformAttorney<OxTransformable>::setTransformedObject(transform, getObjectToBeTransformed());
    m_is_transform_applied = true;
}

OxTransform OxTransformable::getTransform() const
{
    return m_transform;
}

bool OxTransformable::isTransformApplied() const
{
    return m_is_transform_applied;
}

RTtransform OxTransformable::getNativeOptiXTransformHandle() const
{
    return OxTransformAttorney<OxTransformable>::getNativeOptiXTransformHandle(m_transform);
}
