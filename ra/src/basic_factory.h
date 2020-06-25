#ifndef RA_BASIC_FACTORY
#define RA_BASIC_FACTORY

#include <vector>

#include "entity.h"
#include "ra_fwd.h"
#include "program.h"
#include "ray_payloads.h"
#include "geometry_group.h"

namespace ra {

class RaBasicFactory : public RaEntity
{
public:
    static RaBasicFactory* initialize(RaContext const& context);
    static void shutdown();
    static RaBasicFactory* retrieve();

public:
    // required by RaEntity interface
    bool isValid() const override;

    RaContext const& context() const;

    // programs
public:
    RaProgram createProgram(std::string const& source, RaProgram::Source source_type, std::string const& program_name) const;
    
    // buffers
public:
    RaAbstractBuffer createBuffer(RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width) const;
    RaAbstractBuffer createBuffer(RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width, size_t height) const;
    RaAbstractBuffer createBuffer(RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width, size_t height, size_t depth) const;

    // materials
public:
    RaMaterial createMaterial(util::Optional<RaProgram> const& closest_hit_shader,
        util::Optional<RaProgram> const& any_hit_shader, 
        RaRayTypeCollection const& supported_ray_types = RaRayTypeCollection{ RaRayType::unknown }) const;
    RaMaterialAssembly createMaterialAssembly(std::vector<RaMaterial> const& materials) const;
    RaMaterialAssembly createDummyMaterialAssembly() const;

    // geometries
public:
    RaGeometry createGeometry(RaProgram const& intersection_shader, RaProgram const& aabb_shader) const;
    RaGeometry createGeometry(RaProgram const& intersection_shader, RaProgram const& aabb_shader,
        RaMaterialAssembly const& material_assembly) const;
    RaGeometryGroup createGeometryGroup(RaBVHAlgorithm acceleration_structure_construction_algorithm) const;

    // miss shaders
public:
    RaMissShader createMissShader(RaProgram const& miss_shader, 
        RaRayTypeCollection const& supported_ray_types = RaRayTypeCollection{ RaRayType::unknown }) const;
    RaMissShaderAssembly createMissShaderAssembly(std::vector<RaMissShader> const& miss_shaders) const;

    // ray generators
public:
    RaRayGeneratorWithOutputBuffer createRayGenerator(RaProgram const& ray_generation_shader,
        RaAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U) const;

    RaRayGeneratorWithOutputBuffer createRayGenerator(RaProgram const& ray_generation_shader,
        RaMissShaderAssembly const& miss_shader_assembly, RaAbstractBuffer const& output_buffer,
        std::string const& output_buffer_binding_name,
        uint32_t num_rays_x, uint32_t num_rays_y = 1U, uint32_t num_rays_z = 1U,
        uint32_t entry_point_index = 0U) const;

    // scenes
public:
    RaSceneSection createSceneSection(RaBVHAlgorithm acceleration_structure_construction_algorithm) const;
    RaScene createScene() const;

private:
    RaBasicFactory(RaContext const& context);

private:
    RaContext const& m_context;
};

}

#endif
