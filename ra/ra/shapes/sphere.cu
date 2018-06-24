#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtDeclareVariable(optix::float3, location, , "location of sphere");
rtDeclareVariable(float, radius, , "radius of sphere");
rtDeclareVariable(unsigned int, ra_num_materials, "number of materials attached to geometry");

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, "currently traversed ray");

rtDeclareVariable(optix::float3, normal, attribute attrNormal, "normal vector of surface");
rtDeclareVariable(optix::float3, tangent, attribute attrTangent, "tangent vector of surface");
rtDeclareVariable(optix::float2, texcoord, attribute attrTexcoord, "texture coordinate of surface");


__device__ float nz_sgn(float x)
{
    return x >= 0 ? 1.f : -1.f;
}


__device__ float angle_from_sin_and_cos(float s, float c)
{
    return acosf(c)*nz_sgn(s) + M_PIf * static_cast<float>(s < 0);
}

__device__ void setAttributes(float t)
{
    float3 p{ s + t * d };

    {
        // set normal

        float3 n = optix::normalize(n);
        normal.x = n.x; normal.y = n.y; normal.z = n.z;
    }

    {
        // set tangent

        float l = optix::length(p);

        float cos_theta = p.y / l;
        float sin_theta = sqrtf(1.f - cos_theta * cos_theta);

        float cos_phi = p.x / l / sin_theta;
        float sin_phi = p.z / l / sin_theta;

        tangent.x = -sin_phi;
        tangent.y = 0.f;
        tangent.z = cos_phi;


        // set texture coordinates (for now we use equirectangular projection as it is the simplest)
        float theta = 1 - acosf(cos_theta) / M_PIf;    // it is just more natural this way, so the textures can be drawn from lower to upper latitudes
        float phi = angle_from_sin_and_cos(sin_phi, cos_phi) / (2 * M_PIf);

        texcoord.x = theta;
        texcoord.y = phi;
    }

}


RT_PROGRAM void __ra_intersect__(int primitive_id)
{
    float3 o{ ray.origin.x, ray.origin.y, ray.origin.z };
    float3 d{ ray.direction.x, ray.direction.y, ray.direction.z };
    // d = optix::normalize(d);

    float3 s = o - location;
    float oml2 = optix::dot(s, s);
    float oml_d = optix::dot(s, d);

    float D = oml_d * oml_d - oml2;
    if (D >= 0)
    {
        D = sqrt(D);
        float t1 = oml_d - D;
        float t2 = oml_d + D;

        for (unsigned i = 0U; i < ra_num_materials; ++i)
        {
            bool check_second{ true };

            if (rtPotentialIntersection(t1))
            {
                setAttributes(t1);
                if (rtReportInteresection(i)) check_second = false;
            }

            if (check_second)
            {
                if (rtPotentialIntersection(t2))
                {
                    setAttributes(t2);
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
    aabb[2] = location.z - radius;
    aabb[3] = location.x + radius;
    aabb[4] = location.y + radius;
    aabb[5] = location.z + radius;
}