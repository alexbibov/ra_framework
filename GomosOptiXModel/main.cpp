#include <iostream>
#include <tclap/CmdLine.h>

#include "../ox_wrapper/ox_wrapper/init.h"
#include "../ox_wrapper/ox_wrapper/shapes/circle.h"
#include "../ox_wrapper/ox_wrapper/material_assembly.h"
#include "../ox_wrapper/ox_wrapper/ray_casters/parallel_ray_generator.h"
#include "../ox_wrapper/ox_wrapper/scene_section.h"
#include "../ox_wrapper/ox_wrapper/rendering_passes/scattering_rendering_pass.h"
#include "../ox_wrapper/ox_wrapper/materials/black_body.h"

int main(int argc, char* argv[])
{
    try
    {
        TCLAP::CmdLine cmd{ "OX_WRAPPER demonstration demo based on Gomos OptiX project", ' ', "0.1" };

        TCLAP::ValueArg<unsigned int> num_rays{ "r", "num_rays", "Number of rays to generate", true, 1000, "unsigned int" };
        TCLAP::ValueArg<unsigned int> max_recursion_depth{ "d", "max_recursion_depth", 
            "Maximal allowed depth of recursion", false, 20, "unsigned int" };
        TCLAP::ValueArg<std::string> ox_wrapper_path{ "", "ox_lib", "Path to OX_WRAPPER library", true, "", "string" };
        TCLAP::ValueArg<std::string> path_to_settings{ "", "path_to_settings", "Path to OX_WRAPPER settings JSON file", true, "", "string" };
        

        cmd.add(num_rays);
        cmd.add(max_recursion_depth);
        cmd.add(ox_wrapper_path);
        cmd.add(path_to_settings);

        cmd.parse(argc, argv);

        
        // initialize OX_WRAPPER
        {
            ox_wrapper::OxInit ox{ ox_wrapper_path.getValue(), path_to_settings.getValue() };
            ox_wrapper::shapes::OxCircle atmospheric_circle{ ox.context(), 2.f, 1.1f, 1.1f };
            ox_wrapper::shapes::OxCircle planet_circle{ ox.context(), 2.f, 1.1f, 1.f };
            ox_wrapper::ray_casters::OxParallelRayGenerator parallel_ray_generator{ ox.context(), num_rays.getValue(), 2.f, -2.f, -3.14f / 2.f, 2 };

            // define materials
            {
                ox_wrapper::materials::OxBlackBody black_body{ ox.context(), ox_wrapper::OxRayPayloadType::radiance };
                ox_wrapper::OxMaterialAssembly planet_ma{ black_body };
                planet_circle.setMaterialAssembly(planet_ma);
                atmospheric_circle.setMaterialAssembly(ox_wrapper::OxMaterialAssembly{});
            }


            // initialize geometry group corresponding to Earth object 
            ox_wrapper::OxGeometryGroup earth_geometry_group{ ox.context(), ox_wrapper::OxBVHAlgorithm::trbvh };
            {
                earth_geometry_group.beginConstruction();
                earth_geometry_group.addGeometry(atmospheric_circle);
                earth_geometry_group.addGeometry(planet_circle);
                earth_geometry_group.endConstruction();
            }

            // initialize scene section
            ox_wrapper::OxSceneSection scene_section{ parallel_ray_generator, ox_wrapper::OxBVHAlgorithm::trbvh };
            {
                scene_section.beginConstruction();
                scene_section.addGeometryGroup(earth_geometry_group);
                scene_section.endConstruction();
            }

            // initialize render pass
            ox_wrapper::rendering_passes::OxScatteringRenderingPass scattering_pass{ scene_section, 2, max_recursion_depth.getValue(), .01f, num_rays.getValue() };
            
            
            
        }
        
    }
    catch (TCLAP::ArgException& e)
    {
        std::cout << "Error while parsing the command line for argument " << e.argId() << " (" << e.error() << ")" << std::endl;
    }
    catch (ox_wrapper::OxException& e)
    {
        std::cout << std::string{ "ox_wrapper has thrown exception: \"" } +e.what() + "\"" << std::endl;
    }

    std::cout << "alive entities:" << ox_wrapper::OxEntity::aliveEntities() << std::endl;
    return EXIT_SUCCESS;
}