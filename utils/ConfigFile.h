//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigFile.h
// Brief    : Configuration file handler
//==============================================================================

#ifndef GRAPE_CONFIGFILE_H
#define GRAPE_CONFIGFILE_H

#include "ConfigNode.h"

namespace Grape
{

/// \class ConfigFile
/// \ingroup utils
/// \brief configuration file reader/writer
///
/// ConfigFile class provides methods to read a configuration file containing
/// named sections with key-value pairs and optional comments. The following is
/// an example:
///
/// \code
/// float_val = 0; /' entries are allowed outside a section '/
///
/// [section_name]
/// 	integer_val = 2;     /' This is a comment string for the entry '/
/// 	float_val   = 2.0;
/// 	float_array = 1.0 2.0 3.0 4.0;
/// 	string_val = This is a message;
/// 	[sub_section_name]
/// 		float  = 3.0;
///     [/sub_section_name]
/// [/section_name]
/// \endcode
///
/// Rules:
/// - For a key/value entry, the value must be terminated by ';'
/// - Comment must be enclosed within /' '/.
/// - Multiline block comments are supported
/// - Nested comments are not supported
/// - Sections are named and contained within [name][/name] blocks
///   where 'name' identifies the section.
/// - Nested sections are supported
///
/// Internally, the data is represented as a hierarchical tree where each node
/// is a section containing multiple key/value/comment entries and multiple
/// child nodes (subsections).
///
/// <b>Example Program:</b>
/// \include ConfigFileExample.cpp

class GRAPEUTILS_DLL_API ConfigFile
{

public:

    ConfigFile();
    ~ConfigFile();

    /// Load configuration data from file into memory.
    /// \param fileName  The configuration file.
    /// \param env      An optional environment variable (example PATH) containing the
    ///                 search path for files as a colon-separated list (example:
    ///                 .:/usr/local/bin:~/bin). The current directory is searched
    ///                 first for the specified file, before searching through the
    ///                 path specified by the environment variable.
    /// \return true on success, false on error.
    bool load(const std::string &fileName, const std::string &env="");

    /// Save configuration into a file.
    /// \param  fileName  The destination file.
    /// \return  true on success, false on error.
    bool save(const std::string &fileName);

    /// print to an output stream
    /// \param str output stream (example std::cout)
    /// \param lsp number of leading white spaces to print
    void print(std::ostream &s) const;

    /// get pointer access to a section from its path
    /// \param sectionPath  Path to the section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \return             Pointer to the section, or NULL if not found.
    ConfigNode* getConfig(const std::string& sectionPath);

private:
    /// search specified path for file, open file, return resolved path.
    std::string openFileFromPath(const std::string &path, const std::string &fname, std::ifstream &file);

    ConfigFile &operator=(const ConfigFile &conf);
    ConfigFile(const ConfigFile &conf);

private:

    ConfigNode _root;

}; // ConfigFile


} // Grape

#endif // GRAPE_CONFIGFILE_H
