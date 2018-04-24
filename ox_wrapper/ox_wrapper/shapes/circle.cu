#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtDeclareVariable(optix::float2, center, ,);
rtDeclareVariable(float, radius, , "radius of circle");
rtDeclareVariable(unsigned int, num_materials, , "number of materials attached to geometry primitive");

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, "currently traversed ray");

rtDeclareVariable(optix::float3, normal, attribute attrNormal, "Normal of the surface being hit");


RT_PROGRAM void __ox_intersect__(int primitive_id)
{
    float2 s{ ray.origin.x, ray.origin.y };
    float2 d{ ray.direction.x, ray.direction.y };
    
    float2 aux{ s - center };
    float a{ dot(d, d) };
    float b{ dot(aux, d) };
    float c{ dot(aux, aux) - radius * radius };

    float D{ b*b - a*c };
    if (D > 0)
    {
        D = sqrt(D);
        float t1 = (-b - D) / a;
        float t2 = (-b + D) / a;
        
        for (unsigned int i = 0; i < num_materials; ++i)
        {
            bool check_second{ true };

            if (rtPotentialIntersection(t1))
            {
                float2 p{ aux + t1 * d };
                p /= norm3df(p.x, p.y, 0.f);
                normal.x = p.x; normal.y = p.y; normal.z = 0.f;

                if (rtReportIntersection(i))
                    check_second = false;
            }
            
            if (check_second)
            {
                if (rtPotentialIntersection(t2))
                {
                    float2 p{ aux + t2 * d };
                    p /= norm3df(p.x, p.y, 0.f);
                    normal.x = p.x; normal.y = p.y; normal.z = 0.f;

                    rtReportIntersection(i);
                }
            }
        }
    }
}

RT_PROGRAM void __ox_aabb__(int primitive_id, float aabb[6])
{
    aabb[0] = center.x - radius;
    aabb[1] = center.y - radius;
    aabb[2] = -1e-2f;
    aabb[3] = center.x + radius;
    aabb[4] = center.y + radius;
    aabb[5] = 1e-2f;
}