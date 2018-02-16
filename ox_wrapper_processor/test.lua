-- create planet and atmosphere that encloses it
planet_circle = OxCircle.new(
    2.0, --[X-coordinate of the planet center]
    1.1, --[Y-coordinate of the planet center]
    0.9 --[Z-coodinate of the planter center]
)
planet_circle:setStringName("planet_circle_shape")

atmospheric_circle = OxCircle.new(
    2.0, --[Circle center X coordinate]
    1.0, --[Circle center Y coordinate]
    1.0 --[Circle radius]
)
atmospheric_circle:setStringName("atmosphere_circle_shape")


-- define parallel ray generator
--[[
    Parallel ray generator is modelled by setup of linearly aligned
    emitting pixels. 
    Opening is the total length of emittor, position is 
    its horizontal shift (default is 0) and rotation is 
    counter-clockwise rotation around its center (default is 0).
    The last two arguments are the total number of spectral pairs carried
    by emitted rays' payloads (default is 8 meaning 16 frequencies in total)
    and the entry point, to which the generator will be assigned (see OptiX
    documentation for further reading on the entry points and why one may 
    need them).

    The parallel ray generator created below has the following configuration:

                        |-->
                        |-->
                        |-->
    -------(-3)-------(-2)-------(-1)-------(0)-------(1)---------> X
                        |-->
                        |-->
                        |-->
]]
parallel_ray_generator = OxParallelRayGenerator.new(
    100, --[Total number of rays to cast]
    2, --[Opening of the emitter]
    -2, --[Position of the emitter]
    3.14/2, --[Rotation of the emitter]
    2 --[Number of spectral pairs to support]
)
parallel_ray_generator:setStringName("parallel_ray_generator_raycaster")

--[[]
    Black body is material that absorbs all the radiance from any incoming ray
    Below it is used to simulate planet body.
    The constructor of OxBlackBody (inherited from OxMaterial) accepts two arguments.
    The first identifies is an entry from table named OxRayPayloadType and it identifies 
    the type of ray payload to use. The second argument is OxRayType and it determines, what
    kind of rays should be absorbed by the black body
]]
black_body_normal_rays = OxBlackBody.new(OxRayPayloadType["radiance"], OxRayType["unknown"])
black_body_normal_rays:setStringName("black_body_material_normal_rays")
black_body_scattered_rays = OxBlackBody.new(OxRayPayloadType["radiance"], OxRayType["scattered"])
black_body_scattered_rays:setStringName("black_body_material_scattered_rays")

--[[
    All materials have to be grouped into OxMaterialAssembly objects before they
    can be attached to the respective geometries. Below we create material assembly, which
    contains two instances of black body material: one to absorb normal radiance rays and
    one to absorb secondary rays caused by scattering. This material assembly is afterwards attached to
    the planet geometry. Atmospheric circle in turn gets an EMPTY material assembly object. This is a 
    special case introduced to correctly apply RENDERING PASSES (more details about them are found below)
]]
planet_material_assembly = OxMaterialAssembly.new({ black_body_normal_rays, black_body_scattered_rays })
planet_material_assembly:setStringName("planet_material_assembly")
planet_circle:setMaterialAssembly(planet_material_assembly)
atmospheric_circle:setMaterialAssembly(OxMaterialAssembly.new())

earth_geometry_group = OxGeometryGroup.new(OxBVHAlgorithm["trbvh"])
earth_geometry_group:beginConstruction()
earth_geometry_group:addGeometry(atmospheric_circle)
earth_geometry_group:addGeometry(planet_circle)
earth_geometry_group:endConstruction()

scene_section = OxSceneSection.new(OxBVHAlgorithm["trbvh"])
scene_section:beginConstruction()
scene_section:addGeometryGroup(earth_geometry_group)
scene_section:endConstruction()

scattering_rendering_pass = OxScatteringRenderingPass.new(scene_section, parallel_ray_generator, 2, 10, 0.01, 1)
scattering_rendering_pass:render()
