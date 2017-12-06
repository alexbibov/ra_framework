#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtDeclareVariable(optix::float2, center, , "position of center of circle");
rtDeclareVariable(float, radius, , "radius of circle");
rtDeclareVariable(unsigned int, num_materials, , "number of materials attached to geometry primitive");

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, "currently traversed ray");


RT_PROGRAM void __ox_intersect__(int primitive_id)
{
    float2 s{ ray.origin.x, ray.origin.y };
    float2 d{ ray.direction.x, ray.direction.y };

    float a{ dot(s - center, s - center) - radius*radius };
    float b{ dot(s - center, d) };
    float c{ dot(d, d) };

    float D{ sqrt(b*b - a*c) };
    float t{ fminf((-b - D) / a, (-b + D) / a) };

    if (rtPotentialIntersection(t))
    {
        for (unsigned int i = 0; i < num_materials; ++i)
        {
            rtReportIntersection(i);
        }
    }
}

RT_PROGRAM void __ox_aabb__(int primitive_id, float aabb[6])
{
    aabb[0] = center.x - radius;
    aabb[1] = center.y - radius;
    aabb[2] = -1e-5f;
    aabb[3] = center.x + radius;
    aabb[4] = center.y + radius;
    aabb[3] = 1e-5f;
}