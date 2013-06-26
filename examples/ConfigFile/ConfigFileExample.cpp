//==============================================================================
// File     :   ConfigFileExample.cpp
// Brief    :   Example program for ConfigFile class
//==============================================================================

#include "utils/ConfigFile.h"

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    Grape::Config cfg;

    std::ostringstream str;

    str << "key \n=\nvalue\n;\n <!-- \ncomment\n -->"
        << "key2 = value2; <!-- comment2 -->";

    cfg.parse(str.str());
    cfg.print(std::cout);

    return 0;
}

