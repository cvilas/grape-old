//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigNode.h
// Brief    : A node in a configuration tree
//==============================================================================

#include "ConfigNode.h"

namespace Grape
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
    this->node = conf.node;
    this->children = conf.children;
}

//------------------------------------------------------------------------------
ConfigNode &ConfigNode::operator=(const ConfigNode &conf)
//------------------------------------------------------------------------------
{
    if( this != &conf )
    {
        this->node = conf.node;
        this->children = conf.children;
    }
    return *this;
}

//------------------------------------------------------------------------------
void ConfigNode::print(std::ostream &str, unsigned int lsp) const
//------------------------------------------------------------------------------
{
    this->node.print(str, lsp);

    ConstNodeIter it = children.begin();
    ConstNodeIter itEnd = children.end();

    while( it != itEnd )
    {
        unsigned int sp = lsp;
        while(sp) { str << " "; --sp; }

        str << "<" << it->first << ">" << std::endl;
        it->second.print(str,lsp+1);
        str << "<\\" << it->first << ">" << std::endl;

        ++it;
    }
}

//------------------------------------------------------------------------------
bool ConfigNode::removeChild(const std::string& name)
//------------------------------------------------------------------------------
{
    return ( 0 != children.erase( Grape::removeEndWhiteSpace(name) ) );
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

} // Grape
