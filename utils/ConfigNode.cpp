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

#include "ConfigNode.h"

namespace grape
{

//==============================================================================
ConfigNode::ConfigNode()
//==============================================================================
{
}

//------------------------------------------------------------------------------
ConfigNode::~ConfigNode()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
ConfigNode::ConfigNode(const ConfigNode &conf)
//------------------------------------------------------------------------------
{
    this->_entries = conf._entries;
    this->children = conf.children;
}

//------------------------------------------------------------------------------
ConfigNode &ConfigNode::operator=(const ConfigNode &conf)
//------------------------------------------------------------------------------
{
    if( this != &conf )
    {
        this->_entries = conf._entries;
        this->children = conf.children;
    }
    return *this;
}

//------------------------------------------------------------------------------
void ConfigNode::print(std::ostream &str, unsigned int lsp) const
//------------------------------------------------------------------------------
{
    this->_entries.print(str, lsp);

    ConstNodeIter it = children.begin();
    ConstNodeIter itEnd = children.end();

    while( it != itEnd )
    {
        unsigned int sp = lsp;
        while(sp) { str << " "; --sp; }

        str << "[" << it->first << "]" << std::endl;
        it->second.print(str,lsp+1);

        while(sp < lsp) { str << " "; ++sp; }
        str << "[/" << it->first << "]" << std::endl;

        ++it;
    }
}

//------------------------------------------------------------------------------
bool ConfigNode::parse(const std::string& str, std::ostream& errorStream)
//------------------------------------------------------------------------------
{
    // 1. search for section and remove section block into a separate string
    // 2. recurse into section string and call youself
    // 3. extract entries

    // tokenise the str into sections
    std::string remainder(str);

    while( 1 )
    {
        std::string name;
        std::string section;
        int ret = extractSection(remainder, name, section, errorStream);
        if( ret < 0 )
        {
            return false;
        }

        else if( ret == 0 )
        {
            break;
        }

        else if ( ret > 0 )
        {
            ConfigNode node;
            if( node.parse(section, errorStream) )
            {
                children[name] = node;
            }

        } // found section

    } // while

    // extract entries
    return _entries.parse(remainder, errorStream);
}

//------------------------------------------------------------------------------
int ConfigNode::extractSection(std::string& str,
                        std::string& name,
                        std::string& section,
                        std::ostream& errorStream)
//------------------------------------------------------------------------------
{
    // find beginning of a section
    std::string::size_type headerBeginLoc = str.find('[', 0);
    if( headerBeginLoc == std::string::npos )
    {
        return 0;
    }

    // rest of the section header
    std::string::size_type headerEndLoc = str.find(']', headerBeginLoc+1U);
    if( headerEndLoc == std::string::npos )
    {
        errorStream << "[ConfigNode::extractSection]: section header not terminated" << std::endl;
        return -1;
    }

    // extract section name
    name = grape::removeEndWhiteSpace(str.substr(headerBeginLoc+1U, headerEndLoc-headerBeginLoc-1U));

    std::string::size_type seekPos = headerEndLoc+1U;
    std::string::size_type footerBeginLoc = findFooter(str, name, seekPos, errorStream);
    if( footerBeginLoc == std::string::npos )
    {
        errorStream << "[ConfigNode::extractSection]: footer not found for section " << name << std::endl;
        return -1;
    }

    // extract the section
    section = str.substr(headerEndLoc+1U, footerBeginLoc-headerEndLoc-1U);
    str = str.substr(0, headerBeginLoc) + str.substr(seekPos, std::string::npos);
    return 1;
}

//------------------------------------------------------------------------------
std::string::size_type ConfigNode::findFooter(const std::string& str,
                                              const std::string& name,
                                              std::string::size_type& seekPos,
                                              std::ostream& errorStream)
//------------------------------------------------------------------------------
{
    while( 1 )
    {
        // find beginning of footer
        std::string::size_type beginPos = str.find("[/", seekPos);

        if( beginPos == std::string::npos )
        {
            errorStream << "[ConfigNode::findFooter]: footer not found for section " << name << std::endl;
            break;
        }

        beginPos = beginPos + 2U;

        // find end of footer
        seekPos = str.find(']', beginPos);

        if( seekPos == std::string::npos )
        {
            errorStream << "[ConfigNode::findFooter]: found unterminated footer when searching footer for section " << name << std::endl;
            return seekPos;
        }

        // check if this is the footer we seek
        std::string footerName = grape::removeEndWhiteSpace(str.substr(beginPos, seekPos-beginPos));
        seekPos = seekPos + 1U;
        if( footerName == name )
        {
            return beginPos-2U;
        } // correct footer found
    }

    return std::string::npos;
}

//------------------------------------------------------------------------------
void ConfigNode::clear()
//------------------------------------------------------------------------------
{
    _entries.clear();

    /*
    ConstNodeIter it = children.begin();
    ConstNodeIter itEnd = children.end();

    while( it != itEnd )
    {
        it->second.clear();
        ++it;
    }
    */
    children.clear();

}


//------------------------------------------------------------------------------
bool ConfigNode::removeChild(const std::string& name)
//------------------------------------------------------------------------------
{
    return ( 0 != children.erase( grape::removeEndWhiteSpace(name) ) );
}

//------------------------------------------------------------------------------
bool ConfigNode::removeChild(unsigned int n)
//------------------------------------------------------------------------------
{
    if( n < getNumChildren() )
    {
        NodeIter it = children.begin();
        std::advance(it, n);
        children.erase(it);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
ConfigNode* ConfigNode::getChild(const std::string& name)
//------------------------------------------------------------------------------
{
    NodeIter it = children.find(name);
    if( it == children.end() )
    {
        return NULL;
    }
    return &it->second;
}

//------------------------------------------------------------------------------
ConfigNode* ConfigNode::getChild(unsigned int n)
//------------------------------------------------------------------------------
{
    if( n < getNumChildren() )
    {
        NodeIter it = children.begin();
        std::advance(it, n);
        return &it->second;
    }
    return NULL;
}

//------------------------------------------------------------------------------
std::string ConfigNode::getChildName(unsigned int n) const
//------------------------------------------------------------------------------
{
    if( n < getNumChildren() )
    {
        ConstNodeIter it = children.begin();
        std::advance(it, n);
        return it->first;
    }

    return "";
}

} // grape
