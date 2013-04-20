//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : CmdLineArgs.cpp
//==============================================================================

#include "CmdLineArgs.h"

namespace Grape
{
    //------------------------------------------------------------------------------
    CmdLineArgs::CmdLineArgs(int argc, char** argv)
    //------------------------------------------------------------------------------
    {
        // an option starts with '-' and has more than one character
        std::string currOpt; // option currently being processed
        std::string currVal; // value to the current option
        for(int i = 1; i < argc; ++i) 
        {
            
            // get the next argument
            std::string arg(argv[i]);
            unsigned int len = arg.length();
            if( len == 0 )
            {
                continue;
            }
            
            // option starts with a '-'. Can't be valid option if there are no characters after it.
            if( arg[0] == '-' ) 
            {
                if( len < 2 ) 
                {
                    continue;
                }
                currOpt = arg.substr(1, std::string::npos);
                currVal.clear();
            }
            
            // option starts without a '-'. Move this to argument list of previous option
            else 
            {
                currVal += arg + std::string(" ");
                _map[currOpt] = currVal;
            }
        } // for
    }

} // Grape
