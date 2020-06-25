#include <optix.h>
#include <optixu/optixu_math_namespace.h>

RT_CALLABLE_PROGRAM float2 __ra_scattering_default_absorption_factor__(float3 pos, unsigned int spectrum)
{
    return make_float2(.1f, .1f);
}

RT_CALLABLE_PROGRAM float2 __ra_scattering_default_scattering_factor__(float3 pos, unsigned int spectrum)
{
    return make_float2(1.f, 1.f);
}

RT_CALLABLE_PROGRAM float2 __ra_scattering_default_phase_function__(float3 pos,
    float2 incident_direction, float2 scattering_direction, unsigned int spectrum)
{
    return make_float2(1.f, 1.f) / (4.f*M_PIf);
}