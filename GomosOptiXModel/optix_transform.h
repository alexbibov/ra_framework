#ifndef OX_WRAPPER_OPTIX_TRANSFORM_H
#define OX_WRAPPER_OPTIX_TRANSFORM_H

#include "matrix_types.h"

namespace ox_wrapper {

    class OptiXTransform
    {
    public:
        OptiXTransform(mat4x4 const& transform);

        mat4x4 const& transformationMatrix() const;

        /*! Combines this transform with provided transform. More precisely, if this transform 
         is A and provided transform is B, then the result of this operation is A*B
         */
        OptiXTransform operator*(OptiXTransform const& other) const;

    private:
        mat4x4 m_transformation_matrix;
    };

}

#endif
