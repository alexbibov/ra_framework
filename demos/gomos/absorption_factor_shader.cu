#include <optix.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtDeclareVariable(float2, planet_location, , "Location of the center of the planet");
rtDeclareVariable(float, planet_radius, , "Radius of the planet");
rtDeclareVariable(float, atmosphere_thickness, , "Thickness of the atmosphere");


RT_CALLABLE_PROGRAM float2 exponential_decay_absorption(float3 pos, unsigned int spectrum)
{
    float2 _2d_position = make_float2(pos.x, pos.y) - planet_location;
    float height = length(_2d_position) - planet_radius;

    return height < atmosphere_thickness ? make_float2(atmosphere_thickness-height, 2*(atmosphere_thickness-height)) 
        : make_float2(0.f, 0.f);
}

// RT_CALLABLE_PROGRAM float2 __ox_scattering_default_scattering_factor__(float3 pos, unsigned int spectrum)
// {
//     return make_float2(1.f, 1.f);
// }

// RT_CALLABLE_PROGRAM float2 __ox_scattering_default_phase_function__(float3 pos,
//     float2 incident_direction, float2 scattering_direction, unsigned int spectrum)
// {
//     return make_float2(1.f, 1.f) / (4.f*M_PIf);
// }