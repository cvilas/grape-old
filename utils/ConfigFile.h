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
///
/// ConfigFile class provides methods to read a configuration file containing
/// named sections with key-value pairs and optional comments. The format
/// for a configuration entry is as follows:
/// \code
/// [name] = [value] [#comment]
/// \endcode
///
/// Multiple nested subsections are supported. Sub-sections are contained
/// in <name></name> blocks where 'name' identifies the sub-section.
///
/// The following is an example configuration file:
///
/// \code
/// float = 0; # entries are allowed outside a section
///
/// <section_name>
/// 	integer = 2;     # Anything after '#' is a comment string for that entry
/// 	float   = 2.0;
/// 	float_array = 1.0 2.0 3.0 4.0;
/// 	string = This is a message;
/// 	<sub_section_name> # nested subsection
/// 		float  = 3.0;
/// 		<sub_sub_section> # 2nd level nesting
///         </sub_sub_section>
///     </sub_section_name>
/// </section_name>
/// \endcode
///
/// <b>Example Program:</b>
/// \include ConfigFileExample.cpp

class GRAPEUTILS_DLL_API ConfigFile
{
public:
    ConfigFile();
    ~ConfigFile();

private:

    /// \brief A single node in the configuration tree
    class ConfigNode
    {
    public:
        Grape::Config node;
        std::map<std::string/*name*/, ConfigNode> children;
    }; // ConfigNode

}; // ConfigFile

} // Grape

#endif // GRAPE_CONFIGFILE_H
