//==============================================================================
// File     :   ConfigFileExample.cpp
// Brief    :   Example program for ConfigFile class
//==============================================================================

#include "utils/ConfigFile.h"

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    Grape::ConfigFile file(std::cout);

    // load a configuration
    if( !file.load("../examples/ConfigFile/example_config.cfg", "PATH") )
    {
        return -1;
    }

    // add a subsection at the root level
    file.addSection("NewSubSection", "");

    // add another nested subsection
    file.addSection("NewSubSubSection", "NewSubSection");

    // add/modify an entry
    double d = 3.2;
    file.setEntry("NewSubSection::NewSubSubSection", "doubleVal", d, "My double entry");

    // read an entry
    std::string comment;
    double r;
    if( !file.getEntry("NewSubSection::NewSubSubSection", "doubleVal", r, comment) )
    {
        std::cout << "entry not found" << std::endl;
    }
    else
    {
        std::cout << "double entry: \n" << r << "\ncomment: " << comment << std::endl;
    }

    // print configuration
    std::cout << "========== configuration file ======= " << std::endl;
    file.print(std::cout);

    // save configuration
    if( !file.save("saved_config.cfg") )
    {
        std::cout << "save error" << std::endl;
    }

    return 0;

}

