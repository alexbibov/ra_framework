working_directory = OxLibs.OxDirectoryLib.get_current_directory().."/../demos/gomos/"    -- a bit dirty as this is dependent on where the script is run from
                                                                                         -- there should be possibility to get location of the script *itself* (to be implemented)
ox_add_asset_look_up_directory(working_directory)    -- add new directory to look for shaders

num_rays = 100    -- total number of rays to cast
frequency_pairs = 8    -- number of frequency pairs to support (means 4 frquencies in total in this case)
ox_set_context_stack_size(4096)

planet_radius = 13.272916666    -- radius of the planet
atmosphere_thickness = 1    -- thickness of atmosphere
planet_location = float2.new(0.0, -(planet_radius + atmosphere_thickness / 2.0))   -- X and Y coordinates of the center of the planet

-- create planet and atmosphere that encloses it
planet_circle = OxCircle.new(
    planet_location.x,
    planet_location.y,
    planet_radius
)

planet_circle:setStringName("planet_circle_shape")

atmospheric_circle = OxCircle.new(
    planet_location.x,
    planet_location.y,
    planet_radius + atmosphere_thickness
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
    num_rays, --[Total number of rays to cast]
    atmosphere_thickness*0.95, --[Opening of the emitter]
    -(planet_radius + atmosphere_thickness + 1), --[Position of the emitter]
    -3.14/2, --[Rotation of the emitter]
    frequency_pairs --[Number of spectral pairs to support]
)


-- load gas profile and cross section data stored separately in MATLAB v4 file
gomos_data_load_agent = OxMatlabV4.new(working_directory.."gomosdata.mat")
vars = gomos_data_load_agent:getVariables()
num_heights = nil
num_wavelengths = nil
num_gas_profiles = nil
for v = 1, #vars do

    if vars[v].name == "gas_profiles" then

        num_heights = math.tointeger(vars[v].number_of_rows)
        num_gas_profiles = math.tointeger(vars[v].number_of_columns)
        gas_profiles = OxBuffer.new(OxBufferFormat["FLOAT"], OxBufferKind["input"], num_heights, num_gas_profiles)    

    elseif vars[v].name == "cross_sections" then

        num_wavelengths = math.tointeger(vars[v].number_of_rows)
        num_gas_profiles = math.tointeger(vars[v].number_of_columns)
        cross_sections = OxBuffer.new(OxBufferFormat["FLOAT"], OxBufferKind["input"], num_wavelengths, num_gas_profiles)

    end

end
gomos_data_load_agent:load(gas_profiles, 0, OxBufferFormat["FLOAT"], "gas_profiles")
gomos_data_load_agent:load(cross_sections, 0, OxBufferFormat["FLOAT"], "cross_sections")


-- -- write per-spectrum intensities for rays casted by the parallel ray generator
spectral_flux = {}
for i = 1, num_rays*frequency_pairs do
    e = float2.new(1, 1)
    spectral_flux[i] = e
end
parallel_ray_generator:updateSpectralFluxBuffer(spectral_flux)

parallel_ray_generator:setStringName("parallel_ray_generator_raycaster")

-- --[[]
--     Black body is simple material that absorbs all the radiance from any incoming ray.
--     Below this material is used to simulate absorption of the planet body.
--     The constructor of OxBlackBody (inherited from OxMaterial) accepts two arguments.
--     The first must be an entry from table named OxRayPayloadType, which determines
--     the type of ray payload to use. The second argument must be set of OxRayType enumeration
--     values determining, which types of rays must be affected by the black body material.
-- ]]
black_body = OxBlackBody.new(OxRayPayloadType["radiance"], { OxRayType["unknown"], OxRayType["scattered"] })
black_body:setStringName("black_body_material")

-- --[[
--     All materials have to be grouped into OxMaterialAssembly objects before they
--     can be attached to the respective geometries. Below we create material assembly and pack there 
--     the black body material, which was initialized above. 
--     This material assembly is afterwards getting attached to
--     the planet geometry. Atmospheric circle in turn gets an EMPTY material assembly object. This is a 
--     special case introduced to correctly apply RENDERING PASSES (more details about them are found below)
-- ]]
planet_material_assembly = OxMaterialAssembly.new({ black_body })
planet_material_assembly:setStringName("planet_material_assembly")
planet_circle:setMaterialAssembly(planet_material_assembly)

-- --[[
--     Atmospheric circle must behave like a transparent "volumetric" medium, which is not exactly
--     what materials are for. Volumetric behavior can though be simulated by complex combination 
--     of material and miss shaders. Such complex scenarios are usually implemented by rendering passes.
--     Rendering pass is complex object, which takes user-defined ray generator and scene section and
--     patches each geometry in the given scene section, which does not have material shaders provided to
--     act correspondingly to the documented behavior of the rendering pass. Our atmospheric circle must
--     be implemented by scattering rendering pass, so we attach a "dummy" material assembly to it, so that
--     the scattering rendering pass initialized below would not that this geometry (atmospheric_circle) must
--     be patches and behave in accordance with scattering transfer model
-- ]]
atmospheric_circle:setMaterialAssembly(OxMaterialAssembly.new())

-- --[[
--     All geometries must be combined into geometry groups. This is the only way to attach them to scene section,
--     which eventually can be ray-traced. Geometry groups are needed to identify how acceleration structures will
--     be constructed for the given set of geometry objects. The algorithm used to construct acceleration structure
--     is provided to the input of OxGeometryGroup constructor.
-- ]]
earth_geometry_group = OxGeometryGroup.new(OxBVHAlgorithm["none"])
earth_geometry_group:beginConstruction()
earth_geometry_group:addGeometry(atmospheric_circle)
earth_geometry_group:addGeometry(planet_circle)
earth_geometry_group:endConstruction()

-- --[[
--     Geometry groups to be ray-traced must be packed into scene sections, which in turn may include not only geometry
--     groups, but also other scene sections. Scene sections are just higher level abstraction of geometry to be
--     ray-traced and it's main purpose is to define the higher-level bounding volume hierarchy (BVH). Similar to OxGeometryGroup
--     the only parameter accepted by constructor of OxSceneSection is algorithm to use for construction of the corresponding
--     acceleration structure (acceleration structure = BVH in this case)
-- ]]
scene_section = OxSceneSection.new(OxBVHAlgorithm["none"])
scene_section:beginConstruction()
scene_section:addGeometryGroup(earth_geometry_group)
scene_section:endConstruction()


scattering_rendering_pass = OxScatteringRenderingPass.new(
    scene_section,    -- scene section, to which the scattering pass will be applied
    parallel_ray_generator,    -- ray generator employed by the scattering pass
    frequency_pairs,    -- number of spectral pairs supported by the scattering pass (must be equal to that of the ray generator)
    4,   -- maximal depth of recursion
    0.05,    -- ray marching step size
    1    -- number of importance directions used to approximate the scattering integral
)

absorption_factor_shader = OxProgram.new(
    "gomos_absorption_factor_shader.ptx",
    OxProgramSource["file"],
    "gomos_absorption_lut"
)

absorption_factor_shader:setVariableValue("planet_location", planet_location)
absorption_factor_shader:setVariableValue("planet_radius", planet_radius)
absorption_factor_shader:setVariableValue("atmosphere_thickness", atmosphere_thickness)

absorption_factor_shader:setVariableValue("num_wavelengths", num_wavelengths)
absorption_factor_shader:setVariableValue("num_heights", num_heights)
absorption_factor_shader:setVariableValue("num_gas_profiles", num_gas_profiles)

absorption_factor_shader:assignBuffer("gas_profiles", gas_profiles)
absorption_factor_shader:assignBuffer("cross_sections", cross_sections)


-- importance_directions = { float2.new(0.0, 3.14) }
-- for i = 2, frequency_pairs + 1 do
--     importance_directions[i] = float2.new(1.0, 1.0)
-- end
-- scattering_rendering_pass:updateImportanceDirections(importance_directions)

gomos_data_store_agent = OxMatlabV4.new(working_directory.."results.mat")

for pass = 1, math.ceil(num_wavelengths/16) do
    io.write(string.format("pass#%i...", pass))

    absorption_factor_shader:setVariableValue("wavelength_buffer_offset", 16*(pass-1))
    
    scattering_rendering_pass:setAbsorptionProbabilityShader(absorption_factor_shader)
    scattering_rendering_pass:render()    -- execute ray-tracing
    output = parallel_ray_generator:outputBuffer()
    gomos_data_store_agent:save(output, 0, OxBufferFormat["RAY_RADIANCE_PAYLOAD"], "output_pass"..pass)

    io.write("done\n")
end
