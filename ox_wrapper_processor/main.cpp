#include <iostream>
#include <tclap/CmdLine.h>

#include "../ox_wrapper/ox_wrapper/init.h"
#include "../ox_wrapper/ox_wrapper/shapes/circle.h"
#include "../ox_wrapper/ox_wrapper/material_assembly.h"
#include "../ox_wrapper/ox_wrapper/ray_casters/parallel_ray_generator.h"
#include "../ox_wrapper/ox_wrapper/scene_section.h"
#include "../ox_wrapper/ox_wrapper/rendering_passes/scattering_rendering_pass.h"
#include "../ox_wrapper/ox_wrapper/materials/black_body.h"
#include "../ox_wrapper/ox_wrapper/util/lua_support.h"
#include "../ox_wrapper/ox_wrapper/version.h"

#include <windows.h>

BOOL WINAPI ctrlCEnventHandler(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
        exit(EXIT_SUCCESS);
        break;

    default:
        return TRUE;
    }
}


int main(int argc, char* argv[])
{
    try
    {
        TCLAP::CmdLine cmd{ "OX_WRAPPER demonstration demo based on Gomos OptiX project", ' ', "0.1" };

        TCLAP::ValueArg<std::string> lua_script{ "s", "lua_script", "input script written in LUA", false, "", "string" };
        TCLAP::ValueArg<std::string> ox_wrapper_path{ "", "ox_lib", "Path to OX_WRAPPER library", true, "", "string" };
        TCLAP::ValueArg<std::string> path_to_settings{ "", "path_to_settings", "Path to OX_WRAPPER settings JSON file", true, "", "string" };

        cmd.add(lua_script);
        cmd.add(ox_wrapper_path);
        cmd.add(path_to_settings);

        cmd.parse(argc, argv);


        // work with ox_wrapper
        {
            ox_wrapper::OxInit ox{ ox_wrapper_path.getValue(), path_to_settings.getValue() };

            std::string input_lua_script_path = lua_script.getValue();
            if (input_lua_script_path.length())
            {
                ox.executeLuaScriptFromSource(input_lua_script_path);

            }
            else
            {
                SetConsoleCtrlHandler(ctrlCEnventHandler, TRUE);

                ox_wrapper::util::lua_support::LuaState::registerFunction(
                    "clc", []() { system("cls"); }
                );

                std::cout << "OX_WRAPPER v " << OX_WRAPPER_VERSION_MAJOR << "." << OX_WRAPPER_VERSION_MINOR << " "
                    << OX_WRAPPER_VERSION_SUFFIX << " (" << OX_WRAPPER_VERSION_CODENAME << ") Interactive Mode" << std::endl;
                
                std::cout << std::endl <<
                    "   *******   **     **       *******  ******** *******   **     ** ****     **" << std::endl <<
                    "  **/////** //**   **       /**////**/**///// /**////** /**    /**/**/**   /**" << std::endl <<
                    " **     //** //** **        /**   /**/**      /**   /** /**    /**/**//**  /**" << std::endl <<
                    "/**      /**  //***         /******* /******* /*******  /**    /**/** //** /**" << std::endl <<
                    "/**      /**   **/**        /**////  /**////  /**///**  /**    /**/**  //**/**" << std::endl <<
                    "//**     **   ** //**       /**      /**      /**  //** /**    /**/**   //****" << std::endl <<
                    " //*******   **   //**      /**      /********/**   //**//******* /**    //***" << std::endl <<
                    "  ///////   //     //       //       //////// //     //  ///////  //      ///" << std::endl << std::endl;


                while (true)
                {
                    std::cout << ">>";

                    char input_buffer[4096];
                    std::cin.getline(input_buffer, sizeof(input_buffer), '\n');

                    try 
                    {
                        ox_wrapper::util::lua_support::LuaState::executeScript(std::string{ input_buffer });
                    }
                    catch (ox_wrapper::OxException& e)
                    {
                        std::cout << e.what() << std::endl;
                    }

                }
                
            }

            
        }

        
        // initialize OX_WRAPPER
        /*{
            
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
            
            
            
        }*/
        
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