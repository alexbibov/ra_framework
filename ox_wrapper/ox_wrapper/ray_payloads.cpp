#include "ray_payloads.h"

using namespace ox_wrapper;

uint64_t ox_wrapper::rayTypeCollectionTo64BitMask(OxRayTypeCollection const& ray_type_collection)
{
    uint64_t mask{ 0U };
    for (OxRayType rt : ray_type_collection)
    {
        assert(static_cast<unsigned int>(rt) < 64);
        mask |= 0x1ui64 << static_cast<unsigned int>(rt);
    }

    return mask;
}

