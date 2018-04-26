#include <optix.h>
#include <optixu/optixu_vector_types.h>
#include <optixu/optixu_vector_functions.h>
#include <optixu/optixu_math_namespace.h>

#include "ox_wrapper/constants.h"
#include "ox_wrapper/ray_payloads.h"

using namespace ox_wrapper;
using namespace optix;


rtDeclareVariable(rtObject, ox_entry_node, , "Scene entry node");
rtDeclareVariable(unsigned int, dimension, , "Dimension of the output buffer");
rtDeclareVariable(unsigned int, payload_type, , "Type of payload");
rtDeclareVariable(unsigned int, ray_type, , "Type of relaunched rays");
rtDeclareVariable(float, ray_parametric_length, , "Parametric length of recasted ray");

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
rtBuffer<OxRayRadiancePayload, 1> ox_radiance_payload_1d_out;
rtBuffer<OxRayRadiancePayload, 2> ox_radiance_payload_2d_out;
rtBuffer<OxRayRadiancePayload, 3> ox_radiance_payload_3d_out;

rtBuffer<OxRayRadiancePayloadSimple, 1> ox_radiance_payload_simple_1d_out;
rtBuffer<OxRayRadiancePayloadSimple, 2> ox_radiance_payload_simple_2d_out;
rtBuffer<OxRayRadiancePayloadSimple, 3> ox_radiance_payload_simple_3d_out;

rtBuffer<OxRayRadiancePayloadMonochromatic, 1> ox_radiance_payload_monochromatic_1d_out;
rtBuffer<OxRayRadiancePayloadMonochromatic, 2> ox_radiance_payload_monochromatic_2d_out;
rtBuffer<OxRayRadiancePayloadMonochromatic, 3> ox_radiance_payload_monochromatic_3d_out;

rtBuffer<OxRayOcclusionPayload, 1> ox_occlusion_payload_1d_out;
rtBuffer<OxRayOcclusionPayload, 2> ox_occlusion_payload_2d_out;
rtBuffer<OxRayOcclusionPayload, 3> ox_occlusion_payload_3d_out;


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

RT_PROGRAM void __ox_generate__(void)
{
    float3 origin, direction;
    uint3 output_buffer_index;
    unpack_ray(origin, direction, output_buffer_index);
/*
    ox_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[2].x = origin.x;
    ox_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[2].y = origin.y;
    ox_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[3].x = direction.x;
    ox_radiance_payload_1d_out[output_buffer_index.x].spectral_radiance[3].y = direction.y;*/

    Ray current_ray = make_Ray(origin, direction, ray_type, 0.f, ray_parametric_length);

    switch (static_cast<OxRayPayloadType>(payload_type))
    {
    case OxRayPayloadType::radiance:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            OxRayRadiancePayload payload = ox_radiance_payload_1d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            OxRayRadiancePayload payload = ox_radiance_payload_2d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            OxRayRadiancePayload payload = ox_radiance_payload_3d_out[output_buffer_index];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;

    case OxRayPayloadType::radiance_simple:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            OxRayRadiancePayloadSimple payload = ox_radiance_payload_simple_1d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_simple_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            OxRayRadiancePayloadSimple payload = ox_radiance_payload_simple_2d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_simple_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            OxRayRadiancePayloadSimple payload = ox_radiance_payload_simple_3d_out[output_buffer_index];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_simple_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;

    case OxRayPayloadType::monochromatic:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            OxRayRadiancePayloadMonochromatic payload = ox_radiance_payload_monochromatic_1d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_monochromatic_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            OxRayRadiancePayloadMonochromatic payload = ox_radiance_payload_monochromatic_2d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_monochromatic_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            OxRayRadiancePayloadMonochromatic payload = ox_radiance_payload_monochromatic_3d_out[output_buffer_index];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_radiance_payload_monochromatic_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;

    case OxRayPayloadType::occlusion:
        switch (dimension)
        {
        case 1:
        {
            unsigned int idx = output_buffer_index.x;
            OxRayOcclusionPayload payload = ox_occlusion_payload_1d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_occlusion_payload_1d_out[idx] = payload;
            break;
        }

        case 2:
        {
            uint2 idx = make_uint2(output_buffer_index.x, output_buffer_index.y);
            OxRayOcclusionPayload payload = ox_occlusion_payload_2d_out[idx];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_occlusion_payload_2d_out[idx] = payload;
            break;
        }

        case 3:
        {
            OxRayOcclusionPayload payload = ox_occlusion_payload_3d_out[output_buffer_index];
            rtTrace(ox_entry_node, current_ray, payload);
            ox_occlusion_payload_3d_out[output_buffer_index] = payload;
            break;
        }
        }
        break;
    }
}


