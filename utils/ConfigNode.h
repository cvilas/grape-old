//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigNode.h
// Brief    : A node in a configuration tree
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
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
