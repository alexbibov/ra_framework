#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtDeclareVariable(optix::float2, location, , "location of circle");
rtDeclareVariable(float, radius, , "radius of circle");
rtDeclareVariable(unsigned int, ra_num_materials, , "number of materials attached to geometry primitive");

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, "currently traversed ray");

rtDeclareVariable(optix::float3, normal, attribute attrNormal, "Normal of the surface being hit");


RT_PROGRAM void __ra_intersect__(int primitive_id)
{
    float2 s{ ray.origin.x, ray.origin.y };
    float2 d{ ray.direction.x, ray.direction.y };
    // d = optix::normalize(d);

    float2 aux{ s - location };
    // float a{ dot(d, d) };
    float b{ dot(aux, d) };
    float c{ dot(aux, aux) - radius * radius };

    float D{ b*b - /*a**/c };
    if (D > 0)
    {
        D = sqrt(D);
        float t1 = (-b - D)/* / a*/;
        float t2 = (-b + D)/* / a*/;
        
        for (unsigned int i = 0; i < ra_num_materials; ++i)
        {
            bool check_second{ true };

            if (rtPotentialIntersection(t1))
            {
                float2 p{ aux + t1 * d };
                p = optix::normalize(p);
                normal.x = p.x; normal.y = p.y; normal.z = 0.f;

                if (rtReportIntersection(i)) check_second = false;
            }
            
            if (check_second)
            {
                if (rtPotentialIntersection(t2))
                {
                    float2 p{ aux + t2 * d };
                    p = optix::normalize(p);
                    normal.x = p.x; normal.y = p.y; normal.z = 0.f;

                    rtReportIntersection(i);
                }
            }
        }
    }
}

RT_PROGRAM void __ra_aabb__(int primitive_id, float aabb[6])
{
    aabb[0] = location.x - radius;
    aabb[1] = location.y - radius;
    aabb[2] = -1e10f;
    aabb[3] = location.x + radius;
    aabb[4] = location.y + radius;
    aabb[5] = 1e-10f;
}