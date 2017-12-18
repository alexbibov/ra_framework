#ifndef PTX_H
#define PTX_H


// 2D shapes
#define PTX_CIRCLE "ox_wrapper/shapes/ptx/circle.ptx"
//#define PTX_RECTANGLE "ox_wrapper/shapes/ptx/rectangle.ptx"
//#define PTX_TRIANGLE "ox_wrapper/shapes/ptx/triangle.ptx"
//#define PTX_POLYGON "ox_wrapper/shapes/ptx/polygon.ptx"

//3D shapes
//#define PTX_SPHERE "ox_wrapper/shapes/ptx/sphere.ptx"
//#define PTX_BOX "ox_wrapper/shapes/ptx/box.ptx"
//#define PTX_PYRAMID "ox_wrapper/shapes/ptx/pyramid.ptx"
//#define PTX_MESH "ox_wrapper/shapes/ptx/mesh.ptx"

// Ray casters
#define PTX_PARALLEL_RAY_GENERATOR "ox_wrapper/ray_casters/ptx/parallel_ray_generator.ptx"

// Material
#define PTX_SCATTERING_MATERIAL "ox_wrapper/materials/ptx/scattering_material.ptx"


// Standartized shader string names
#define OX_SHADER_ENTRY_INTERSECTION   "__ox_intersect__"
#define OX_SHADER_ENTRY_BOUNDING_BOX   "__ox_aabb__"
#define OX_SHADER_ENTRY_RAY_GENERATION "__ox_generate__"
#define OX_SHADER_ENTRY_CLOSEST_HIT    "__ox_closest_hit__"
#define OX_SHADER_ENTRY_ANY_HIT        "__ox_any_hit"
#define OX_SHADER_ENTRY_MISS           "__ox_miss__"
#define OX_SHADER_ENTRY_SELECTOR       "__ox_selector__"
#define OX_SHADER_ENTRY_CALLABLE       "__ox_callable__"


#endif