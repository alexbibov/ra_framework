#include <iostream>
#include <tclap/CmdLine.h>

#include "init.h"
#include "shapes/circle.h"
#include "material_assembly.h"
#include "ray_casters/parallel_ray_generator.h"
#include "scene_section.h"
#include "rendering_passes/scattering_rendering_pass.h"
#include "materials/black_body.h"
#include "util/lua_support.h"
#include "version.h"

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
        TCLAP::CmdLine cmd{ "RA demonstration demo based on Gomos OptiX project", ' ', "0.1" };

        TCLAP::ValueArg<std::string> lua_script{ "s", "lua_script", "input script written in LUA", false, "", "string" };
        TCLAP::ValueArg<std::string> ra_path{ "", "ra_lib", "Path to RA library", true, "", "string" };
        TCLAP::ValueArg<std::string> path_to_settings{ "", "path_to_settings", "Path to RA settings JSON file", true, "", "string" };

        cmd.add(lua_script);
        cmd.add(ra_path);
        cmd.add(path_to_settings);

        cmd.parse(argc, argv);


        // work with ra
        {
            ra::RaInit ox{ ra_path.getValue(), path_to_settings.getValue() };

            std::string input_lua_script_path = lua_script.getValue();
            if (input_lua_script_path.length())
            {
                ox.executeLuaScriptFromSource(input_lua_script_path);

            }
            else
            {
                SetConsoleCtrlHandler(ctrlCEnventHandler, TRUE);

                ra::util::lua_support::LuaState::registerFunction(
                    "clc", []() { system("cls"); }
                );

                std::cout << "RA v " << RA_VERSION_MAJOR << "." << RA_VERSION_MINOR << " "
                    << RA_VERSION_SUFFIX << " (" << RA_VERSION_CODENAME << ") Interactive Mode" << std::endl;
                
                std::cout << std::endl << R"TTT(
                              _____                    _____          
                             /\    \                  /\    \         
                            /::\    \                /::\    \        
                           /::::\    \              /::::\    \       
                          /::::::\    \            /::::::\    \      
                         /:::/\:::\    \          /:::/\:::\    \     
                        /:::/__\:::\    \        /:::/__\:::\    \    
                       /::::\   \:::\    \      /::::\   \:::\    \   
                      /::::::\   \:::\    \    /::::::\   \:::\    \  
                     /:::/\:::\   \:::\____\  /:::/\:::\   \:::\    \ 
                    /:::/  \:::\   \:::|    |/:::/  \:::\   \:::\____\
                    \::/   |::::\  /:::|____|\::/    \:::\  /:::/    /
                     \/____|:::::\/:::/    /  \/____/ \:::\/:::/    / 
                           |:::::::::/    /            \::::::/    /  
                           |::|\::::/    /              \::::/    /   
                           |::| \::/____/               /:::/    /    
                           |::|  ~|                    /:::/    /     
                           |::|   |                   /:::/    /      
                           \::|   |                  /:::/    /       
                            \:|   |                  \::/    /        
                             \|___|                   \/____/     
                )TTT" << std::endl;

                while (true)
                {
                    std::cout << ">>";

                    char input_buffer[4096];
                    std::cin.getline(input_buffer, sizeof(input_buffer), '\n');

                    try 
                    {
                        ra::util::lua_support::LuaState::executeScript(std::string{ input_buffer });
                    }
                    catch (ra::RaException& e)
                    {
                        std::cout << e.what() << std::endl;
                    }

                }
                
            }

            
        }
        
    }
    catch (TCLAP::ArgException& e)
    {
        std::cout << "Error while parsing the command line for argument " << e.argId() << " (" << e.error() << ")" << std::endl;
    }
    catch (ra::RaException& e)
    {
        std::cout << std::string{ "ra has thrown exception: \"" } +e.what() + "\"" << std::endl;
    }

    return EXIT_SUCCESS;
}