#include "optix_transform.h"
#include <immintrin.h>

using namespace ox_wrapper;

OptiXTransform::OptiXTransform(OptiXContext const& optix_context, mat4x4 const& transform):
    HasContractWithOptiXContext{ optix_context }
{
    RTtransform native_handle;
    logOptiXContextError(rtTransformCreate(nativeOptiXContextHandle(), &native_handle));

    m_native_transform.reset(native_handle,
    [this](RTtransform t)->void
    {
        logOptiXContextError(rtTransformDestroy(t));
    });


    float t[16] = {
        transform._11, transform._12, transform._13, transform._14,
        transform._21, transform._22, transform._23, transform._24,
        transform._31, transform._32, transform._33, transform._34,
        transform._41, transform._42, transform._43, transform._44,
    };
    logOptiXContextError(rtTransformSetMatrix(m_native_transform.get(), 0, t, NULL));
}

mat4x4 ox_wrapper::OptiXTransform::transformationMatrix() const
{
    float t[16];
    logOptiXContextError(rtTransformGetMatrix(m_native_transform.get(), 0, t, NULL));

    return mat4x4{
        t[0], t[1], t[2], t[3],
        t[4], t[5], t[6], t[7],
        t[8], t[9], t[10], t[11],
        t[12], t[13], t[14], t[15]
    };
}

OptiXTransform OptiXTransform::operator*(OptiXTransform const& other) const
{
    mat4x4 A = this->transformationMatrix();
    mat4x4 B = other.transformationMatrix();
    

    __m256 a, b;


    return *this;
}
