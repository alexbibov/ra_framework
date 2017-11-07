#ifndef OX_WRAPPER_OPTIX_TRANSFORM_H
#define OX_WRAPPER_OPTIX_TRANSFORM_H

#include "has_contract_with_optix_context.h"
#include "matrix_types.h"
#include "optix.h"

#include <memory>

namespace ox_wrapper {

    class OptiXTransform : public HasContractWithOptiXContext
    {
    public:
        OptiXTransform(OptiXContext const& optix_context, mat4x4 const& transform);

        mat4x4 transformationMatrix() const;

        /*! Combines this transform with provided transform. More precisely, if this transform 
         is A and provided transform is B, then the result of this operation is A*B
         */
        OptiXTransform operator*(OptiXTransform const& other) const;

    private:
        std::shared_ptr<RTtransform_api> m_native_transform;

    };

}

#endif
