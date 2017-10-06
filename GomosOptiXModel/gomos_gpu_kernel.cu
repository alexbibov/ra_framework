#include <optix.h>
#include <optix_math.h>
#include "../CUDA/v9.0/include/vector_types.h"
#include "ray_payload.h"

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, "Current ray");
rtDeclareVariable(gomos_optix_model::RayPayload, payload, rtPayload, "Ray payload structure");
rtDeclareVariable(float, intersection_distance, rtIntersectionDistance, "Ray intersection distance");

rtDeclareVariable(unsigned int, ray_index, rtLaunchIndex, "Ray generation index");

rtDeclareVariable(float, planet_radius, , "Radius of the planet");
rtDeclareVariable(float, height, , "Height of the atmospheric layer");
rtDeclareVariable(float, density, , "Density of the atmospheric layer");
rtDeclareVariable(unsigned int, recursion_depth, , "Depth of ray tracing recursion");

rtDeclareVariable(rtObject, top_object, , "Scene graph entry object");

rtBuffer<gomos_optix_model::RayPayload> ray_marching_buffer;
rtBuffer<gomos_optix_model::ConvergedRayPayload> converged_rays_buffer;

// Intersection program for the atmospheric layer
RT_PROGRAM void atmospheric_layer_intersection_program(int)
{
    float const R = planet_radius + height;
    float sTd = dot(ray.origin, ray.direction);
    float dTd = dot(ray.direction, ray.direction);
    float sTs = dot(ray.origin, ray.origin);
    float D = sTd*sTd - dTd*(sTs - R*R);

    if (D >= 0)
    {
        float t[2] = { (-sTd - D) / dTd, (-sTd + D) / dTd };
        
        bool check_second_intersection_point{ true };
        if (rtPotentialIntersection(t[0]))
        {
            check_second_intersection_point = !rtReportIntersection(0);
        }
            
        if (check_second_intersection_point && rtPotentialIntersection(t[1]))
        {
            rtReportIntersection(0);
        }
    }
}


// Bounding box program for atmospheric layer
RT_PROGRAM void atmospheric_layer_bbox_program(int, float result[6])
{
    // by default we assume coordinate system with the origin located in the center point of the planet

    float const R = planet_radius + height;
    result[0] = result[1] = result[2] = -R;
    result[3] = result[4] = result[5] = R;
}


// Any hit program for the atmospheric layer
RT_PROGRAM void atmospheric_layer_any_hit_program(void)
{
    rtIgnoreIntersection();    // we are not interested in some random hit between the ray and geometry, only the closest hit matters
}


// Closest hit program for the atmosperic layer
RT_PROGRAM void atmospheric_layer_closest_hit_program(void)
{
    /*payload.intensity = height > 0 ? payload.intensity * optix::expf(-density*intersection_distance) : 0.f;
    float3 next_ray_marching_step = ray.origin + intersection_distance*ray.direction;

    if (payload.generation > 0)
    {
        optix::Ray next_ray{ next_ray_marching_step,  ray.direction, 0, 0.f };
        gomos_optix_model::RayPayload next_payload{ payload.intensity, next_ray_marching_step, --payload.generation };
    }*/
}


// Miss program is used to test ray convergence
RT_PROGRAM void atmosperic_layer_miss_program(void)
{
    //payload.converged = true;
}


// Ray generation program, runs once per iteration of the ray marching
RT_PROGRAM void ray_marching_step(void)
{
    /*float3 new_ray_origin{ output_buffer.x, output_buffer.y, output_buffer.z };
    optix::Ray new_ray{ new_ray_origin, float3{ 0.f, 1.f, 0.f }, 0, 0.f };

    */
}