#ifndef OX_WRAPPER_BASIC_FACTORY
#define OX_WRAPPER_BASIC_FACTORY

#include <vector>

#include "entity.h"
#include "fwd.h"
#include "program.h"
#include "ray_payloads.h"
#include "geometry_group.h"

namespace ox_wrapper {

class OxBasicFactory : public OxEntity
{
public:
    static OxBasicFactory* initialize(OxContext const& context);
    static void shutdown();
    static OxBasicFactory* retrieve();

public:
    // required by OxEntity interface
    bool isValid() const override;

    // programs
public:
    OxProgram createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const;
    
    // buffers
public:
    enum class OxBufferFormat
    {
        FLOAT, FLOAT2, FLOAT3, FLOAT4,
        INT, INT2, INT3, INT4,
        UINT, UINT2, UINT3, UINT4,
        CHAR, CHAR2, CHAR3, CHAR4,
        UCHAR, UCHAR2, UCHAR3, UCHAR4,
        RAY_RADIANCE_PAYLOAD, RAY_RADIANCE_PAYLOAD_SIMPLE,
        RAY_RADIANCE_PAYLOAD_MONOCHROMATIC,
        RAY_OCCLUSION_PAYLOAD
    };

    OxAbstractBuffer createBuffer(OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width) const;
    OxAbstractBuffer createBuffer(OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height) const;
    OxAbstractBuffer createBuffer(OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) const;

    // materials
public:
    OxMaterial createMaterial(util::Optional<OxProgram> const& closest_hit_shader,
        util::Optional<OxProgram> const& any_hit_shader, OxRayType ray_type = OxRayType::unknown) const;
    OxMaterialAssembly createMaterialAssembly(std::vector<OxMaterial> const& materials) const;

    // geometries
public:
    OxGeometry createGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader) const;
    OxGeometry createGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader,
        OxMaterialAssembly const& material_assembly) const;
    OxGeometryGroup createGeometryGroup(OxBVHAlgorithm acceleration_structure_construction_algorithm) const;

    // miss shaders
public:
    OxMissShader createMissShader(OxProgram const& miss_shader, OxRayType ray_type = OxRayType::unknown) const;
    OxMissShaderAssembly createMissShaderAssembly(std::vector<OxMissShader> const& miss_shaders) const;

    // ray generators
public:
    OxRayGeneratorWithOutputBuffer createRayGenerator(OxProgram const& ray_generation_shader,
        OxAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U) const;

    OxRayGeneratorWithOutputBuffer createRayGenerator(OxProgram const& ray_generation_shader,
        OxMissShaderAssembly const& miss_shader_assembly, OxAbstractBuffer const& output_buffer,
        std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U) const;

    // scenes
public:
    OxSceneSection createSceneSection(OxRayGenerator const& ray_generator, OxBVHAlgorithm acceleration_structure_construction_algorithm) const;
    OxScene createScene() const;

private:
    OxBasicFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}

#endif