#ifndef OX_WRAPPER_TRANSFORM_H
#define OX_WRAPPER_TRANSFORM_H

#include "fwd.h"
#include "contract_with_context.h"
#include "util/matrix_types.h"
#include "optix.h"
#include "entity.h"

#include <memory>

namespace ox_wrapper {

template<typename T> class OxTransformAttorney;

class OxTransform : public OxContractWithOxContext, public OxEntity
{
    friend class OxTransformAttorney<OxTransformable>;

public:
    OxTransform(OxContext const& optix_context);
    OxTransform(OxContext const& optix_context, util::mat4x4 const& transform);

    void setMatrix(util::mat4x4 const& transformation_matrix);
    util::mat4x4 getMatrix() const;

    /*! Combines this transform with provided transformation matrix.
     More precisely, if this transform is A and transform described by the transformation matrix is B, then
     after this operation this transform is given by A*B
    */
    OxTransform& operator*(util::mat4x4 const& transformation_matrix);

    // required by OxEntity interface
    bool isValid() const override;

private:
    std::shared_ptr<RTtransform_api> m_native_transform;
};

template<>
class OxTransformAttorney<OxTransformable>
{
    friend class OxTransformable;

    static RTtransform getNativeOptiXTransformHandle(OxTransform const& parent_transform)
    {
        return parent_transform.m_native_transform.get();
    }

    static void setTransformedObject(OxTransform const& parent_transform, RTobject transformed_object)
    {
        parent_transform.throwOptiXContextError(rtTransformSetChild(parent_transform.m_native_transform.get(), transformed_object));
    }
};

}

#endif
