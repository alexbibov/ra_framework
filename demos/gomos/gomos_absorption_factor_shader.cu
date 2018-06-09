#include <optix_world.h>

using namespace optix;

rtDeclareVariable(float, wavelength_buffer_offset, , "Offset in wavelength buffer");
rtDeclareVariable(float, num_wavelengths, , "Total number of wavelengths");
rtDeclareVariable(float, num_heights, , "Number of atmospheric layers");
rtDeclareVariable(float, num_gas_profiles, , "Number of gas profiles");

rtDeclareVariable(float2, planet_location, , "Location of the center of the planet");
rtDeclareVariable(float, planet_radius, , "Radius of the planet");
rtDeclareVariable(float, atmosphere_thickness, , "Thickness of the atmosphere");

rtBuffer<float, 2> gas_profiles;
rtBuffer<float, 2> cross_sections;

RT_CALLABLE_PROGRAM float2 gomos_absorption_lut(float3 pos, unsigned int spectrum)
{
    unsigned height = static_cast<unsigned>(round((pos.y + atmosphere_thickness*.5f)*num_heights) - 1);

    float2 absorption_coefficient = make_float2(0.f, 0.f);
    for(int i = 0; i < static_cast<unsigned>(num_gas_profiles); ++i)
    {
        float X = gas_profiles[make_uint2(height, i)];
        float C1 = 0.f, C2 = 0.f;
        if(2*spectrum + wavelength_buffer_offset < num_wavelengths)
        {
            C1 = cross_sections[make_uint2(static_cast<unsigned>(2*spectrum + wavelength_buffer_offset), i)];
        }
        if(2*spectrum + 1 + wavelength_buffer_offset < num_wavelengths)
        {
            C2 = cross_sections[make_uint2(static_cast<unsigned>(2*spectrum + 1 + wavelength_buffer_offset), i)];
        }
        
       absorption_coefficient += X*make_float2(C1, C2);
    }
    return 400*absorption_coefficient;
}

