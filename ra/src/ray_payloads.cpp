#include "ray_payloads.h"

using namespace ra;

uint64_t ra::rayTypeCollectionTo64BitMask(RaRayTypeCollection const& ray_type_collection)
{
    uint64_t mask{ 0U };
    for (RaRayType rt : ray_type_collection)
    {
        assert(static_cast<unsigned int>(rt) < 64);
        mask |= 0x1ui64 << static_cast<unsigned int>(rt);
    }

    return mask;
}

