#ifndef RA_PGET_PAYLOAD_H
#define RA_PGET_PAYLOAD_H

#include <optixu/optixu_vector_types.h>

namespace ra::pget {

struct Payload final
{
    float irradiance;
    float depth;
    unsigned int pass_id;
};

}

#endif
