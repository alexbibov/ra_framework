
#ifndef PTX_H 
#define PTX_H

// Standardized shader string names
#define OX_SHADER_ENTRY_INTERSECTION   "__ra_intersect__"
#define OX_SHADER_ENTRY_BOUNDING_BOX   "__ra_aabb__"
#define OX_SHADER_ENTRY_RAY_GENERATION "__ra_generate__"
#define OX_SHADER_ENTRY_CLOSEST_HIT    "__ra_closest_hit__"
#define OX_SHADER_ENTRY_ANY_HIT        "__ra_any_hit__"
#define OX_SHADER_ENTRY_MISS           "__ra_miss__"
#define OX_SHADER_ENTRY_SELECTOR       "__ra_selector__"
#define OX_SHADER_ENTRY_CALLABLE       "__ra_callable__"
// cuda
#define PTX_PGET_DEVICE "D:/Repositories/Personal/cmake_builds/ra/ptx/cuda/pget_device.ptx"
#endif