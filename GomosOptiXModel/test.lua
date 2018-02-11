atmospheric_circle = OxCircle.new(2.0, 1.0, 1.0)
atmospheric_circle:setStringName("atmosphere_circle_shape")

planet_circle = OxCircle.new(2.0, 1.1, 1.0)
planet_circle:setStringName("planet_circle_shape")

parallel_ray_generator = OxParallelRayGenerator.new(100, 2, -2, 3.14/2, 2)
parallel_ray_generator:setStringName("parallel_ray_generator_raycaster")

black_body = OxBlackBody.new(OxRayPayloadType["radiance"])
black_body:setStringName("black_body_material")

planet_material_assembly = OxMaterialAssembly.new({ black_body })
planet_material_assembly:setStringName("planet_material_assembly")
planet_circle:setMaterialAssembly(planet_material_assembly)
atmospheric_circle:setMaterialAssembly(OxMaterialAssembly.new())

earth_geometry_group = OxGeometryGroup.new(OxBVHAlgorithm["trbvh"])
earth_geometry_group:beginConstruction()
earth_geometry_group:addGeometry(atmospheric_circle)
earth_geometry_group:addGeometry(planet_circle)
earth_geometry_group:endConstruction()

scene_section = OxSceneSection.new(parallel_ray_generator, OxBVHAlgorithm["trbvh"])
scene_section:beginConstruction()
scene_section:addGeometryGroup(earth_geometry_group)
scene_section:endConstruction()

scattering_rendering_pass = OxScatteringRenderingPass.new(scene_section, 2, 10, 0.01, 1)
scattering_rendering_pass:render()
