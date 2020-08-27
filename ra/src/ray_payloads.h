#ifndef RA_RAY_PAYLOADS
#define RA_RAY_PAYLOADS

#include "util/static_vector.h"

namespace ra {

<<<<<<< HEAD
enum class RaRayPayloadType : unsigned int
{
    radiance = 0,
    radiance_simple = 1,
    monochromatic = 2,
    occlusion = 3
};

enum class RaRayType : unsigned int
{
    unknown,
    shadow,
    scattered,
    reflected,
    count
};

using RaRayTypeCollection = util::StaticVector<RaRayType, static_cast<size_t>(RaRayType::count)>;

uint64_t rayTypeCollectionTo64BitMask(RaRayTypeCollection const& ray_type_collection);
=======
    enum class RaRayPayloadType : unsigned int
    {
        radiance = 0,
        radiance_simple = 1,
        monochromatic = 2,
        occlusion = 3
    };

    enum class RaRayType : unsigned int
    {
        unknown = 0U,
        shadow = 1U,
        scattered = 2U,
        reflected = 3U,
        count
    };

    using RaRayTypeCollection = util::StaticVector<RaRayType, static_cast<size_t>(RaRayType::count)>;

    uint64_t rayTypeCollectionTo64BitMask(RaRayTypeCollection const& ray_type_collection);
>>>>>>> 4859c6e... Begin implementation of textures

}

#include "ray_radiance_payload.h"
#include "ray_occlusion_payload.h"

#endif
