#include <iostream>
#include <tclap/CmdLine.h>

#include "../ra/ra/init.h"
#include "../ra/ra/shapes/circle.h"
#include "../ra/ra/material_assembly.h"
#include "../ra/ra/ray_casters/parallel_ray_generator.h"
#include "../ra/ra/scene_section.h"
#include "../ra/ra/rendering_passes/scattering_rendering_pass.h"
#include "../ra/ra/materials/black_body.h"
#include "../ra/ra/util/lua_support.h"
#include "../ra/ra/version.h"

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
        TCLAP::ValueArg<std::string> ra_path{ "", "ox_lib", "Path to OX_WRAPPER library", true, "", "string" };
        TCLAP::ValueArg<std::string> path_to_settings{ "", "path_to_settings", "Path to OX_WRAPPER settings JSON file", true, "", "string" };

        cmd.add(lua_script);
        cmd.add(ra_path);
        cmd.add(path_to_settings);

        cmd.parse(argc, argv);


        // work with ra
        {
            ra::OxInit ox{ ra_path.getValue(), path_to_settings.getValue() };

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

                std::cout << "OX_WRAPPER v " << OX_WRAPPER_VERSION_MAJOR << "." << OX_WRAPPER_VERSION_MINOR << " "
                    << OX_WRAPPER_VERSION_SUFFIX << " (" << OX_WRAPPER_VERSION_CODENAME << ") Interactive Mode" << std::endl;
                
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
                    catch (ra::OxException& e)
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
    catch (ra::OxException& e)
    {
        std::cout << std::string{ "ra has thrown exception: \"" } +e.what() + "\"" << std::endl;
    }

    return EXIT_SUCCESS;
}