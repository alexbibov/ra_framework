#ifndef OX_WRAPPER_BASIC_FACTORY
#define OX_WRAPPER_BASIC_FACTORY

#include <vector>

#include "entity.h"
#include "ox_wrapper_fwd.h"
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

    OxContext const& context() const;

    // programs
public:
    OxProgram createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const;
    
    // buffers
public:
    OxAbstractBuffer createBuffer(OxBasicBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width) const;
    OxAbstractBuffer createBuffer(OxBasicBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height) const;
    OxAbstractBuffer createBuffer(OxBasicBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) const;

    // materials
public:
    OxMaterial createMaterial(util::Optional<OxProgram> const& closest_hit_shader,
        util::Optional<OxProgram> const& any_hit_shader, 
        OxRayTypeCollection const& supported_ray_types = OxRayTypeCollection{ OxRayType::unknown }) const;
    OxMaterialAssembly createMaterialAssembly(std::vector<OxMaterial> const& materials) const;
    OxMaterialAssembly createDummyMaterialAssembly() const;

    // geometries
public:
    OxGeometry createGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader) const;
    OxGeometry createGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader,
        OxMaterialAssembly const& material_assembly) const;
    OxGeometryGroup createGeometryGroup(OxBVHAlgorithm acceleration_structure_construction_algorithm) const;

    // miss shaders
public:
    OxMissShader createMissShader(OxProgram const& miss_shader, 
        OxRayTypeCollection const& supported_ray_types = OxRayTypeCollection{ OxRayType::unknown }) const;
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
    OxSceneSection createSceneSection(OxBVHAlgorithm acceleration_structure_construction_algorithm) const;
    OxScene createScene() const;

private:
    OxBasicFactory(OxContext const& context);

private:
    OxContext const& m_context;
};

}

#endif
