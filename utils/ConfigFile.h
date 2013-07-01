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
/// - Comment block, if present, must be after the corresponding key/value entry
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

    ConfigFile(std::ostream& errorStream = std::cerr);
    ~ConfigFile();

    // -------------------------------- file IO -------------------------------

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
    void print(std::ostream &str) const;

    // ------------------- configuration management -------------------------------

    /// Add a new section node in the configuration tree. Use getSection() to modify section
    /// after adding.
    /// \param sectionName Name of the section to add
    /// \param parentPath  Path to the parent section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \return true on success, false if path is incorrect
    /// \see getSection.
    bool addSection(const std::string &sectionName, const std::string &parentPath);

    /// remove a section node in the configuration tree
    /// \param sectionPath  Path to the section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \return             true on success, false is section doesn't exist.
    bool removeSection(const std::string &sectionPath);

    /// get pointer access to a section from its path.
    /// \param sectionPath  Path to the section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \return             Pointer to the section, or NULL if not found.
    ConfigNode* getSection(const std::string& sectionPath);

    /// Set value for an existing key or add a new key/value/comment triplet.
    /// \note The object of type T must have the following ostream
    /// redirection operator defined in order for this to work:
    /// \code
    /// std::ostream& operator<<(std::ostream &, const T &);
    /// \endcode
    /// \param sectionPath  Path to the section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \param key    An identifier name for the entry.
    /// \param value  The value assigned to the name.
    /// \param comment An optional comment string
    /// \return true on success, false if invalid path
    template<class T>
    bool setEntry(const std::string &sectionPath, const std::string& key, const T &value, const std::string& comment="");

    /// Read an entry by key.
    /// \note The object of type T must have the following istream
    /// redirection operator defined in order for this to work:
    /// \code
    /// std::istream& operator>>(std::istream &, const T &);
    /// \endcode
    /// \param sectionPath  Path to the section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \param key     The identifier name for the entry.
    /// \param value   The value associated with the key.
    /// \param comment  The comment associated with the entry
    /// \return true if entry key was found. False is returned if entry key is not found, in which
    /// case the contents of value and comment parameters are undefined.
    template<class T>
    bool getEntry(const std::string &sectionPath, const std::string& key, T &value, std::string& comment);

    /// Remove an entry by key
    /// \param sectionPath  Path to the section from the root, separated by ::. For
    ///                     instance Car::Toyota::Camry specifies a subsection "Camry"
    ///                     within a section "Toyota" which is itself a subsection of
    ///                     "Car". Set this to empty string to specify root level of the
    ///                     configuration.
    /// \param key     The identifier name for the entry.
    bool removeEntry(const std::string &sectionPath, const std::string &key);

private:
    /// search specified path for file, open file, return resolved path.
    std::string openFileFromPath(const std::string &path, const std::string &fname, std::ifstream &file);

    ConfigFile &operator=(const ConfigFile &conf);
    ConfigFile(const ConfigFile &conf);

private:

    ConfigNode _root;
    std::ostream& _errorStream;

}; // ConfigFile


//==============================================================================
template<class T>
bool ConfigFile::setEntry(const std::string &sectionPath, const std::string& key, const T &value, const std::string& comment)
//==============================================================================
{
    ConfigNode* pNode = getSection(sectionPath);
    if( pNode == NULL )
    {
        _errorStream << "[ConfigFile::setEntry]: Section not found" << std::endl;
        return false;
    }
    pNode->getEntries()->setEntry(key, value, comment);
    return true;
}

//------------------------------------------------------------------------------
template<class T>
bool ConfigFile::getEntry(const std::string &sectionPath, const std::string& key, T &value, std::string& comment)
//------------------------------------------------------------------------------
{
    ConfigNode* pNode = getSection(sectionPath);
    if( pNode == NULL )
    {
        _errorStream << "[ConfigFile::getEntry]: Section not found" << std::endl;
        return false;
    }
    return pNode->getEntries()->getEntry(key, value, comment);
}

} // Grape

#endif // GRAPE_CONFIGFILE_H
