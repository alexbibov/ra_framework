#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

rtDeclareVariable(float2, atmosphere_center, , "Nominal center of atmosphere");
rtDeclareVariable(float, atmosphere_radius, , "Nominal radius of atmosphere");

RT_CALLABLE_PROGRAM float2 __ox_scattering_material_default_absorption_factor__(float3 pos, unsigned int spectrum)
{
    return make_float2(1.f, 1.f);
}

RT_CALLABLE_PROGRAM float2 __ox_scattering_material_default_scattering_factor__(float3 pos, unsigned int spectrum)
{
    return make_float2(1.f, 1.f);
}

RT_CALLABLE_PROGRAM float2 __ox_scattering_material_default_phase_funciton__(float3 pos,
    float2 incident_direction, float3 scattering_direction, unsigned int spectrum)
{
    return make_float2(1.f, 1.f) / (4.f*M_PIf);
}