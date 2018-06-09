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
                        ox_wrapper::util::lua_support::LuaState::executeScript(std::string{ input_buffer });
                    }
                    catch (ox_wrapper::OxException& e)
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
    catch (ox_wrapper::OxException& e)
    {
        std::cout << std::string{ "ox_wrapper has thrown exception: \"" } +e.what() + "\"" << std::endl;
    }

    return EXIT_SUCCESS;
}