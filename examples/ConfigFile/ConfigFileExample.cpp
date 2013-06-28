//==============================================================================
// File     :   ConfigFileExample.cpp
// Brief    :   Example program for ConfigFile class
//==============================================================================

#include "utils/ConfigFile.h"

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    Grape::ConfigNode cfg;

    std::ostringstream str;

    str << "[Section] key1 = value1; /' comment1 '/ key2 = value2; /' comment2 '/ [SubSection] key3 = value3; [/SubSection] [/Section]";

    cfg.parse(str.str());
    cfg.print(std::cout);

    return 0;
}

