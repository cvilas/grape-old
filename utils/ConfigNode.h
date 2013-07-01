//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigNode.h
// Brief    : A node in a configuration tree
//==============================================================================

#ifndef GRAPE_CONFIGNODE_H
#define GRAPE_CONFIGNODE_H

#include "Config.h"

namespace Grape
{

/// \class ConfigNode
/// \ingroup utils
/// \brief A section (node) in the configuration tree
///
/// Example program:
/// see ConfigFileExample.cpp
class GRAPEUTILS_DLL_API ConfigNode
{
public:
    ConfigNode();
    ~ConfigNode();
    ConfigNode &operator=(const ConfigNode &conf);
    ConfigNode(const ConfigNode &conf);

    /// print to an output stream
    /// \param str output stream (example std::cout)
    /// \param lsp number of leading white spaces to print
    void print(std::ostream &str, unsigned int lsp=0) const;

    /// read in the node key/value/comments and children from a stream.
    /// The data should be formatted as follows
    /// \code
    /// key = value; /' comment '/
    /// [section]
    ///     key2 = value2;
    /// [/section]
    /// \endcode
    /// The comment blocks are optional.
    /// \param str input stream
    /// \param errorStream stream to output parse error messages to (default is standard error stream).
    /// \return true if data read successfully.
    bool parse(const std::string& str, std::ostream& errorStream = std::cerr);

    // ------------------ node management --------------------

    /// Clear all contents
    void clear();

    /// \return number of children of this node
    unsigned int getNumChildren() const { return children.size(); }

    /// add/modify a child node by name
    void addChild(const std::string& name, const ConfigNode& node) { children[name] = node; }

    /// remove a child node by name
    /// \return true on success, false if child not found
    bool removeChild(const std::string& name);

    /// remove child by index
    /// \note This index has nothing to do with the sequence in which the child was added.
    /// It is an index into the internal map pre-sorted to improve efficiency in looking
    /// up a node by name
    /// \param n The numerical index (starts at 0).
    /// \return true on success, false if child not found
    /// \see getNumChildren
    bool removeChild(unsigned int n);

    /// Get pointer access to a child by name
    /// \return pointer to child node, NULL if child not found
    ConfigNode* getChild(const std::string& name);

    /// Get pointer access to a child by index
    /// \return pointer to child node, NULL if child not found
    /// \see getNumChildren
    ConfigNode* getChild(unsigned int n);

    /// Get the name of a child
    std::string getChildName(unsigned int n) const;

    /// Get pointer access to all key/value/comment entries of this node
    /// \return pointer to entries list
    Config* getEntries() { return &_entries; }

private:
    /// extract the first subsection found in the string. The input string is
    /// modified to contain the remainder of the string
    /// \param str input string
    /// \param name extracted section name
    /// \param section extracted section string
    /// \param errorStream output error stream
    /// \return -1 on error, 0 if no section found, 1 if section found.
    int extractSection(std::string& str, std::string& name, std::string& section,
                       std::ostream& errorStream);

    /// Find starting position of footer for a section
    /// \param str input string
    /// \param name name of the section
    /// \param seekPos search start position. on return this is updated to next position
    ///                after end of footer
    /// \param errorStream output error stream
    /// \return std::string::npos on error, otherwise start of footer position
    std::string::size_type findFooter(const std::string& str, const std::string& name,
                                      std::string::size_type& seekPos, std::ostream& errorStream);
private:
    Grape::Config _entries;
    std::map<std::string/*name*/, ConfigNode> children;

    typedef std::map<std::string/*key*/, ConfigNode>::iterator       NodeIter;
    typedef std::map<std::string/*key*/, ConfigNode>::const_iterator ConstNodeIter;

}; // ConfigNode

} // Grape

#endif // GRAPE_CONFIGNODE_H
