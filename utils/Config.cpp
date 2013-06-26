//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : Config.cpp
//==============================================================================

#include "Config.h"

namespace Grape
{

//------------------------------------------------------------------------------
std::string Config::getEntryKey(unsigned int n) const
//------------------------------------------------------------------------------
{
    if( n < getNumEntries() )
    {
        ConstEntryIter it = _entries.begin();
        std::advance(it, n);
        return it->first;
    }

    return "";
}

//------------------------------------------------------------------------------
bool Config::removeEntry(unsigned int n)
//------------------------------------------------------------------------------
{
    if( n < getNumEntries() )
    {
        EntryIter it = _entries.begin();
        std::advance(it, n);
        _entries.erase(it);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
void Config::print(std::ostream &str, unsigned int lsp) const
//------------------------------------------------------------------------------
{
    ConstEntryIter it = _entries.begin();
    ConstEntryIter itEnd = _entries.end();

    while( it != itEnd )
    {
        unsigned int sp = lsp;
        while(sp) { str << " "; --sp; }

        str << it->first << " = " << it->second.value << ";";
        if( it->second.comment.length() )
        {
            str << " <!-- " << it->second.comment << " -->";
        }
        str << std::endl;

        ++it;
    }
}

//------------------------------------------------------------------------------
bool Config::parse(const std::string& str, std::ostream& errorStream)
//------------------------------------------------------------------------------
{
    std::string::size_type seekPos = 0;

    while( 1 )
    {
        // where does the next 'value' end
        std::string::size_type valueEnd = str.find(';', seekPos);
        if(valueEnd == std::string::npos)
        {
            break;
        }

        // extract key/value pair
        std::string keyValPair = str.substr(seekPos, valueEnd-seekPos);
        seekPos = valueEnd + 1U;

        // break into key and value
        std::string::size_type keyEnd = keyValPair.find('=');
        if( keyEnd == std::string::npos )
        {
            errorStream << "[Config::parse]: Unexpected ; or string not formatted as key = value;" << std::endl;
            return false;
        }

        std::string key = keyValPair.substr(0, keyEnd);
        key = Grape::removeEndWhiteSpace(key);
        if( key.length() == 0 )
        {
            errorStream << "[Config::parse]: missing key" << std::endl;
            return false;
        }

        std::string value = keyValPair.substr(keyEnd+1U, keyValPair.length() );
        value = Grape::removeEndWhiteSpace(value);
        if( value.length() == 0 )
        {
            errorStream << "[Config::parse]: missing value string for (key: "
                      << key << ")." << std::endl;
            return false;

        }

        if(  value.find('=') != std::string::npos)
        {
            errorStream << "[Config::parse]: found = within value string for (key: "
                      << key << ")." << std::endl;
            return false;
        }

        // do we have a comment block
        std::string comment = "";
        std::string::size_type commentStart = str.find("<!--", seekPos);
        std::string testWhiteSpace = str.substr(seekPos, commentStart-seekPos);
        testWhiteSpace = Grape::removeEndWhiteSpace(testWhiteSpace);
        if( testWhiteSpace.length() == 0 )
        {
            commentStart += 4U;

            // comment belongs to this key/value
            std::string::size_type commentEnd = str.find("-->", commentStart);
            if( commentEnd == std::string::npos )
            {
                errorStream << "[Config::parse]: comment not terminated properly" << std::endl;
                return false;
            }
            comment = str.substr(commentStart, commentEnd-commentStart);
            comment = Grape::removeEndWhiteSpace(comment);
            seekPos = commentEnd + 3U;
        }

        // add it
        setEntry(key, value, comment);

    } // while pending key-value exists

    return true;
}

} // Grape
