#ifndef PTX_H
#define PTX_H


// 2D shapes
#define PTX_CIRCLE "ra/shapes/ptx/circle.ptx"
//#define PTX_RECTANGLE "ra/shapes/ptx/rectangle.ptx"
//#define PTX_TRIANGLE "ra/shapes/ptx/triangle.ptx"
//#define PTX_POLYGON "ra/shapes/ptx/polygon.ptx"

//3D shapes
#define PTX_SPHERE "ra/shapes/ptx/sphere.ptx"
//#define PTX_BOX "ra/shapes/ptx/box.ptx"
//#define PTX_PYRAMID "ra/shapes/ptx/pyramid.ptx"
//#define PTX_MESH "ra/shapes/ptx/mesh.ptx"

// Ray casters
#define PTX_PARALLEL_RAY_GENERATOR "ra/ray_casters/ptx/parallel_ray_generator.ptx"
#define PTX_RECASTER_GENERATOR "ra/ray_casters/ptx/recaster_ray_generator.ptx"

// Rendering passes
#define PTX_SCATTERING_RENDERING_PASS "ra/rendering_passes/ptx/scattering_rendering_pass.ptx"
#define PTX_SCATTERING_RENDERING_PASS_DEFAULT_SHADER_CONFIG "ra/rendering_passes/ptx/scattering_rendering_pass_default_shader_config.ptx"

// Materials
#define PTX_BLACK_BODY_MATERIAL "ra/materials/ptx/black_body.ptx"


// Standardized shader string names
#define OX_SHADER_ENTRY_INTERSECTION   "__ra_intersect__"
#define OX_SHADER_ENTRY_BOUNDING_BOX   "__ra_aabb__"
#define OX_SHADER_ENTRY_RAY_GENERATION "__ra_generate__"
#define OX_SHADER_ENTRY_CLOSEST_HIT    "__ra_closest_hit__"
#define OX_SHADER_ENTRY_ANY_HIT        "__ra_any_hit__"
#define OX_SHADER_ENTRY_MISS           "__ra_miss__"
#define OX_SHADER_ENTRY_SELECTOR       "__ra_selector__"
#define OX_SHADER_ENTRY_CALLABLE       "__ra_callable__"


#endif