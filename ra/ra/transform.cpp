#include "transform.h"
#include "context.h"
#include <immintrin.h>

using namespace ra;

OxTransform::OxTransform(OxContext const& optix_context):
    OxContractWithOxContext{ optix_context }
{
    RTtransform native_handle;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformCreate(nativeOptiXContextHandle(), &native_handle));
    m_native_transform.reset(native_handle, 
        [this](RTtransform t)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformDestroy(t));
    });

    float t[16] = {
        1., 0., 0., 0.,
        0., 1., 0., 0.,
        0., 0., 1., 0.,
        0., 0., 0., 1. };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformSetMatrix(native_handle, 0, t, t));
}

OxTransform::OxTransform(OxContext const& optix_context, util::mat4x4 const& transform):
    OxContractWithOxContext{ optix_context }
{
    RTtransform native_handle;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformCreate(nativeOptiXContextHandle(), &native_handle));

    m_native_transform.reset(native_handle,
    [this](RTtransform t)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformDestroy(t));
    });


    float t[16] = {
        transform._11, transform._12, transform._13, transform._14,
        transform._21, transform._22, transform._23, transform._24,
        transform._31, transform._32, transform._33, transform._34,
        transform._41, transform._42, transform._43, transform._44,
    };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformSetMatrix(m_native_transform.get(), 0, t, NULL));
}

void OxTransform::setMatrix(util::mat4x4 const& transformation_matrix)
{
    float t[16] = {
        transformation_matrix._11, transformation_matrix._12, transformation_matrix._13, transformation_matrix._14,
        transformation_matrix._21, transformation_matrix._22, transformation_matrix._23, transformation_matrix._24,
        transformation_matrix._31, transformation_matrix._32, transformation_matrix._33, transformation_matrix._34,
        transformation_matrix._41, transformation_matrix._42, transformation_matrix._43, transformation_matrix._44,
    };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformSetMatrix(m_native_transform.get(), 0, t, NULL));
}

util::mat4x4 ra::OxTransform::getMatrix() const
{
    float t[16];
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformGetMatrix(m_native_transform.get(), 0, t, NULL));

    return util::mat4x4{
        t[0], t[1], t[2], t[3],
        t[4], t[5], t[6], t[7],
        t[8], t[9], t[10], t[11],
        t[12], t[13], t[14], t[15]
    };
}

OxTransform& OxTransform::operator*(util::mat4x4 const& transformation_matrix)
{
    util::mat4x4 A = this->getMatrix();
    util::mat4x4 const& B = transformation_matrix;
    
    alignas(32) float a12[8] = {
        A._11, A._12, A._13, A._14,
        A._21, A._22, A._23, A._24
    };

    alignas(32) float a34[8] = {
        A._31, A._32, A._33, A._34,
        A._41, A._42, A._43, A._44
    };

    alignas(32) float b12[8] = {
        B._11, B._21, B._31, B._41,
        B._12, B._22, B._32, B._42,
    };

    alignas(32) float b34[8] = {
        B._13, B._23, B._33, B._43,
        B._14, B._24, B._34, B._44,
    };

    alignas(32) float dst11_12_33_34_22_21_44_43[8];
    alignas(32) float dst13_14_31_32_24_23_42_41[8];

    __m256 _a12, _a34, _b12, _b34;
    __m256 _dst11_12_33_34_22_21_44_43;
    __m256 _dst13_14_31_32_24_23_42_41;
    __m256 aux;
    _a12 = _mm256_load_ps(a12); _a34 = _mm256_load_ps(a34);
    _b12 = _mm256_load_ps(b12); _b34 = _mm256_load_ps(b34);

    // calculate d11, d22, d33, and d44
    _dst11_12_33_34_22_21_44_43 = _mm256_dp_ps(_a12, _b12, 0xF1);
    aux = _mm256_dp_ps(_a34, _b34, 0xF4);
    _dst11_12_33_34_22_21_44_43 = _mm256_xor_ps(_dst11_12_33_34_22_21_44_43, aux);

    // calculate d12, d34, d21, and d43
    __m256 _b21 = _mm256_permute2f128_ps(_b12, _b12, 0x1);
    __m256 _b43 = _mm256_permute2f128_ps(_b34, _b34, 0x1);
    
    aux = _mm256_dp_ps(_a12, _b21, 0xF2);
    _dst11_12_33_34_22_21_44_43 = _mm256_xor_ps(_dst11_12_33_34_22_21_44_43, aux);

    aux = _mm256_dp_ps(_a34, _b43, 0xF8);
    _dst11_12_33_34_22_21_44_43 = _mm256_xor_ps(_dst11_12_33_34_22_21_44_43, aux);

    // calculate d13, d24, d31, and d42
    _dst13_14_31_32_24_23_42_41 = _mm256_dp_ps(_a12, _b34, 0xF1);
    aux = _mm256_dp_ps(_a34, _b12, 0xF4);
    _dst13_14_31_32_24_23_42_41 = _mm256_xor_ps(_dst13_14_31_32_24_23_42_41, aux);

    // calculate d14, d23, d32, and d41
    aux = _mm256_dp_ps(_a12, _b43, 0xF2);
    _dst13_14_31_32_24_23_42_41 = _mm256_xor_ps(_dst13_14_31_32_24_23_42_41, aux);

    aux = _mm256_dp_ps(_a34, _b21, 0xF8);
    _dst13_14_31_32_24_23_42_41 = _mm256_xor_ps(_dst13_14_31_32_24_23_42_41, aux);

    // store results
    _mm256_store_ps(dst11_12_33_34_22_21_44_43, _dst11_12_33_34_22_21_44_43);
    _mm256_store_ps(dst13_14_31_32_24_23_42_41, _dst13_14_31_32_24_23_42_41);

    float res[16] = {
        dst11_12_33_34_22_21_44_43[0], dst11_12_33_34_22_21_44_43[1], dst13_14_31_32_24_23_42_41[0], dst13_14_31_32_24_23_42_41[1],
        dst11_12_33_34_22_21_44_43[5], dst11_12_33_34_22_21_44_43[4], dst13_14_31_32_24_23_42_41[5], dst13_14_31_32_24_23_42_41[4],
        dst13_14_31_32_24_23_42_41[2], dst13_14_31_32_24_23_42_41[3], dst11_12_33_34_22_21_44_43[2], dst11_12_33_34_22_21_44_43[3],
        dst13_14_31_32_24_23_42_41[7], dst13_14_31_32_24_23_42_41[6], dst11_12_33_34_22_21_44_43[7], dst11_12_33_34_22_21_44_43[6]
    };

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtTransformSetMatrix(m_native_transform.get(), 0, res, NULL));

    return *this;
}

bool OxTransform::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res = rtTransformValidate(m_native_transform.get()));
    return res == RT_SUCCESS;
}
