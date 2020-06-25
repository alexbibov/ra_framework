
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
// materials
#define PTX_BLACK_BODY "D:/Repos/ra_framework/ra/src/materials/ptx/black_body.ptx"
// ray_casters
#define PTX_PARALLEL_RAY_GENERATOR "D:/Repos/ra_framework/ra/src/ray_casters/ptx/parallel_ray_generator.ptx"
// ray_casters
#define PTX_RECASTER_GENERATOR "D:/Repos/ra_framework/ra/src/ray_casters/ptx/recaster_generator.ptx"
// rendering_passes
#define PTX_SCATTERING_RENDERING_PASS "D:/Repos/ra_framework/ra/src/rendering_passes/ptx/scattering_rendering_pass.ptx"
// rendering_passes
#define PTX_SCATTERING_RENDERING_PASS_DEFAULT_SHADER_CONFIG "D:/Repos/ra_framework/ra/src/rendering_passes/ptx/scattering_rendering_pass_default_shader_config.ptx"
// shapes
#define PTX_CIRCLE "D:/Repos/ra_framework/ra/src/shapes/ptx/circle.ptx"
// shapes
#define PTX_SPHERE "D:/Repos/ra_framework/ra/src/shapes/ptx/sphere.ptx"
#endif