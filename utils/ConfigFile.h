//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigFile.h
// Brief    : Configuration file handler
//==============================================================================

#ifndef GRAPE_CONFIGFILE_H
#define GRAPE_CONFIGFILE_H

#include "Config.h"

namespace Grape
{

/// \brief configuration file reader/writer
class ConfigFile
{
public:
    ConfigFile();

private:

    /// \brief A single node in the configuration tree
    class ConfigNode
    {
    public:
        Grape::Config node;
        std::map<std::string/*name*/, Config> children;
    }; // ConfigNode

}; // ConfigFile

} // Grape

#endif // GRAPE_CONFIGFILE_H
