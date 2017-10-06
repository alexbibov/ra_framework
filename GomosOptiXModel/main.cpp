#include <iostream>
#include "tclap/CmdLine.h"

int main(int argc, char* argv[])
{
    try
    {

    }
    catch (TCLAP::ArgException& e)
    {
        std::cout << "Error while parsing the command line for argument " << e.argId() << " (" << e.error() << ")" << std::endl;
    }
    return EXIT_SUCCESS;
}