//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : Config.cpp
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
            str << " /' " << it->second.comment << " '/";
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
        std::string::size_type commentStart = str.find("/'", seekPos);
        if( commentStart != std::string::npos )
        {
            std::string testWhiteSpace = str.substr(seekPos, commentStart-seekPos);
            testWhiteSpace = Grape::removeEndWhiteSpace(testWhiteSpace);
            if( testWhiteSpace.length() == 0 )
            {
                commentStart += 2U;

                // comment belongs to this key/value
                std::string::size_type commentEnd = str.find("'/", commentStart);
                if( commentEnd == std::string::npos )
                {
                    errorStream << "[Config::parse]: comment not terminated properly" << std::endl;
                    return false;
                }
                comment = str.substr(commentStart, commentEnd-commentStart);
                comment = Grape::removeEndWhiteSpace(comment);
                seekPos = commentEnd + 2U;
            }
        }

        // add it
        setEntry(key, value, comment);

    } // while pending key-value exists

    return true;
}

} // Grape
