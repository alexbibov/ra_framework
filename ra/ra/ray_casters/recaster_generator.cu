#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ra/constants.h"
#include "ra/ray_payloads.h"

using namespace ra;
using namespace optix;


rtDeclareVariable(rtObject, ra_entry_node, , "Scene entry node");
rtDeclareVariable(unsigned int, dimension, , "Dimension of the output buffer");
rtDeclareVariable(unsigned int, payload_type, , "Type of payload");
rtDeclareVariable(unsigned int, ray_type, , "Type of relaunched rays");
rtDeclareVariable(float, ray_parametric_length, , "Parametric length of recasted ray");
rtDeclareVariable(uint3, problem_size, , "Original size of the problem");

rtDeclareVariable(unsigned int, index, rtLaunchIndex, "Thread index");


/*! Traverse backup is a standard buffer, which enables implementation of multi-pass ray-tracing algorithms.
The buffer has to be organized as follows:
1st element contains number of vectors stored in the buffer, hereinafter denoted by N
The rest of the buffer must contain at least 6*N floats and N unsigned integers ordered in 9-element
tuples with the first 3 elements being floating point numbers determining the origin of the
corresponding ray, succeeding 3 elements being floating point components of the ray's direction, and
the last 3 unsigned integers determining three-dimensional index of the ray in its original launch grid
*/
rtBuffer<unsigned int, 1> traverse_backup_buffer;


// supported output buffers
rtBuffer<RaRayRadiancePayload, 1> ra_radiance_payload_1d_out;
rtBuffer<RaRayRadiancePayload, 2> ra_radiance_payload_2d_out;
rtBuffer<RaRayRadiancePayload, 3> ra_radiance_payload_3d_out;

rtBuffer<RaRayRadiancePayloadSimple, 1> ra_radiance_payload_simple_1d_out;
rtBuffer<RaRayRadiancePayloadSimple, 2> ra_radiance_payload_simple_2d_out;
rtBuffer<RaRayRadiancePayloadSimple, 3> ra_radiance_payload_simple_3d_out;

rtBuffer<RaRayRadiancePayloadMonochromatic, 1> ra_radiance_payload_monochromatic_1d_out;
rtBuffer<RaRayRadiancePayloadMonochromatic, 2> ra_radiance_payload_monochromatic_2d_out;
rtBuffer<RaRayRadiancePayloadMonochromatic, 3> ra_radiance_payload_monochromatic_3d_out;

rtBuffer<RaRayOcclusionPayload, 1> ra_occlusion_payload_1d_out;
rtBuffer<RaRayOcclusionPayload, 2> ra_occlusion_payload_2d_out;
rtBuffer<RaRayOcclusionPayload, 3> ra_occlusion_payload_3d_out;


__device__ void unpack_ray(float3& origin, float3& direction, uint3& output_buffer_index)
{
    origin.x = __uint_as_float(traverse_backup_buffer[1 + 9 * index]);
    origin.y = __uint_as_float(traverse_backup_buffer[2 + 9 * index]);
    origin.z = __uint_as_float(traverse_backup_buffer[3 + 9 * index]);

    direction.x = __uint_as_float(traverse_backup_buffer[4 + 9 * index]);
    direction.y = __uint_as_float(traverse_backup_buffer[5 + 9 * index]);
    direction.z = __uint_as_float(traverse_backup_buffer[6 + 9 * index]);

    output_buffer_index.x = traverse_backup_buffer[7 + 9 * index];
    output_buffer_index.y = traverse_backup_buffer[8 + 9 * index];
    output_buffer_index.z = traverse_backup_buffer[9 + 9 * index];
}

RT_PROGRAM void __ra_generate__(void)
{
    float3 origin, direction;
    uint3 output_buffer_index;
    unpack_ray(origin, direction, output_buffer_index);
/*
    ra_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[2].x = origin.x;
    ra_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[2].y = origin.y;
    ra_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[3].x = direction.x;
    ra_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[3].y = direction.y;*/

    Ray current_ray = make_Ray(origin, direction, ray_type, 0.f, ray_parametric_length);

    switch (static_cast<RaRayPayloadType>(payload_type))
    {
    case RaRayPayloadType::radiance:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            RaRayRadiancePayload payload = ra_radiance_payload_1d_out[idx];
            payload.tracing_depth_and_aux.z = idx + 1;
            ++payload.tracing_depth_and_aux.w;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_radiance_payload_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            RaRayRadiancePayload payload = ra_radiance_payload_2d_out[idx];
            payload.tracing_depth_and_aux.z = idx.y*problem_size.x + idx.x + 1;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_radiance_payload_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            RaRayRadiancePayload payload = ra_radiance_payload_3d_out[output_buffer_index];
            
            payload.tracing_depth_and_aux.z =
                output_buffer_index.z*problem_size.x*problem_size.y
                + output_buffer_index.y*problem_size.x + output_buffer_index.x + 1;
            
            rtTrace(ra_entry_node, current_ray, payload);

            ra_radiance_payload_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;

    case RaRayPayloadType::radiance_simple:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            RaRayRadiancePayloadSimple payload = ra_radiance_payload_simple_1d_out[idx];
            payload.tracing_depth_and_aux.z = idx + 1;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_radiance_payload_simple_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            RaRayRadiancePayloadSimple payload = ra_radiance_payload_simple_2d_out[idx];
            payload.tracing_depth_and_aux.z = idx.y*problem_size.x + idx.x + 1;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_radiance_payload_simple_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            RaRayRadiancePayloadSimple payload = ra_radiance_payload_simple_3d_out[output_buffer_index];

            payload.tracing_depth_and_aux.z =
                output_buffer_index.z*problem_size.x*problem_size.y
                + output_buffer_index.y*problem_size.x + output_buffer_index.x + 1;
            
            rtTrace(ra_entry_node, current_ray, payload);
            
            ra_radiance_payload_simple_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;

    case RaRayPayloadType::monochromatic:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            RaRayRadiancePayloadMonochromatic payload = ra_radiance_payload_monochromatic_1d_out[idx];
            payload.tracing_depth_and_aux.z = idx + 1;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_radiance_payload_monochromatic_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            RaRayRadiancePayloadMonochromatic payload = ra_radiance_payload_monochromatic_2d_out[idx];
            payload.tracing_depth_and_aux.z = idx.y*problem_size.x + idx.x + 1;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_radiance_payload_monochromatic_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            RaRayRadiancePayloadMonochromatic payload = ra_radiance_payload_monochromatic_3d_out[output_buffer_index];

            payload.tracing_depth_and_aux.z =
                output_buffer_index.z*problem_size.x*problem_size.y
                + output_buffer_index.y*problem_size.x + output_buffer_index.x + 1;

            rtTrace(ra_entry_node, current_ray, payload);
            
            ra_radiance_payload_monochromatic_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;

    case RaRayPayloadType::occlusion:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            RaRayOcclusionPayload payload = ra_occlusion_payload_1d_out[idx];
            // payload.tracing_depth_and_aux.z = idx;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_occlusion_payload_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            RaRayOcclusionPayload payload = ra_occlusion_payload_2d_out[idx];
            // payload.tracing_depth_and_aux.z = idx.y*problem_size.x + idx.x;
            rtTrace(ra_entry_node, current_ray, payload);
            ra_occlusion_payload_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            RaRayOcclusionPayload payload = ra_occlusion_payload_3d_out[output_buffer_index];

            /*payload.tracing_depth_and_aux.z =
                output_buffer_index.z*problem_size.x*problem_size.y
                + output_buffer_index.y*problem_size.x + output_buffer_index.x;*/

            rtTrace(ra_entry_node, current_ray, payload);
            
            ra_occlusion_payload_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;
    }
}


