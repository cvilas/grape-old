//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigFile.cpp
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

#include "ConfigFile.h"
#include <fstream>
#include <time.h>

namespace grape
{

//==============================================================================
ConfigFile::ConfigFile(std::ostream& errorStream)
//==============================================================================
    : _errorStream(errorStream)
{
}

//------------------------------------------------------------------------------
ConfigFile::~ConfigFile()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
bool ConfigFile::load(const std::string &fileName, const std::string &env)
//------------------------------------------------------------------------------
{
    std::ifstream file;
    std::string cpath, path;
    char* envPath = getenv( env.c_str() );
    path = ".:";
    if( env.length() && (envPath != NULL) )
    {
        path += (envPath);
    }

    cpath = openFileFromPath(path, fileName, file);
    if( !file.is_open() )
    {
        _errorStream << "[ConfigFile::load]: Unable to open file" << std::endl;
        return false;
    }

    std::string data;

    data.assign( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()) );
    file.close();

    return _root.parse(data, _errorStream);
}

//------------------------------------------------------------------------------
bool ConfigFile::save(const std::string &fileName)
//------------------------------------------------------------------------------
{
    std::ofstream file(fileName.c_str(), std::ios_base::out|std::ios_base::trunc);
    if( !file.is_open() )
    {
        _errorStream << "[ConfigFile::save]: Unable to open file " << fileName << std::endl;
        return false;
    }

    time_t t = time( NULL );
    file << "# Configuration file saved on " << ctime( &t ) << std::endl;
    print(file);
    file.close();
    return true;
}

//------------------------------------------------------------------------------
void ConfigFile::print(std::ostream &s) const
//------------------------------------------------------------------------------
{
    _root.print(s,0);
}

//------------------------------------------------------------------------------
ConfigNode* ConfigFile::getSection(const std::string& path)
//------------------------------------------------------------------------------
{
    std::string remainingPath( grape::removeEndWhiteSpace(path) );
    ConfigNode* pNode = &_root;

    if( path.length() )
    {
        std::string::size_type loc = std::string::npos;

        do
        {
            loc = remainingPath.find("::");
            std::string currPath = remainingPath.substr(0, loc);
            pNode = pNode->getChild(currPath);
            remainingPath = remainingPath.substr(loc+2U);
        }while( (loc != std::string::npos) && (pNode != NULL) );

    } // path specified

    return pNode;
}

//------------------------------------------------------------------------------
bool ConfigFile::addSection(const std::string &sectionName, const std::string &parentPath)
//------------------------------------------------------------------------------
{
    ConfigNode* pNode = getSection(parentPath);
    if( pNode == NULL )
    {
        _errorStream << "[ConfigFile::addSection]: " << parentPath << " not found" << std::endl;
        return false;
    }

    pNode->addChild(sectionName, ConfigNode());
    return true;
}

//------------------------------------------------------------------------------
bool ConfigFile::removeSection(const std::string &sectionPath)
//------------------------------------------------------------------------------
{
    std::string::size_type pos = sectionPath.find_last_of("::");
    if( pos == std::string::npos )
    {
        _errorStream << "[ConfigFile::removeSection]: invalid path" << std::endl;
        return false;
    }

    std::string parentPath = sectionPath.substr(0, pos);
    std::string sectionName = sectionPath.substr(pos+2U);

    ConfigNode* pNode = getSection(parentPath);
    if( pNode == NULL )
    {
        _errorStream << "[ConfigFile::removeSection]: " << parentPath << " not found" << std::endl;
        return false;
    }
    return pNode->removeChild(sectionName);
}

//------------------------------------------------------------------------------
bool ConfigFile::removeEntry(const std::string &sectionPath, const std::string &key)
//------------------------------------------------------------------------------
{
    ConfigNode* pNode = getSection(sectionPath);
    if( pNode == NULL )
    {
        _errorStream << "[ConfigFile::removeEntry]: " << sectionPath << " not found" << std::endl;
        return false;
    }

    Config* pEntries = pNode->getEntries();
    return pEntries->removeEntry(key);
}


//------------------------------------------------------------------------------
std::string ConfigFile::openFileFromPath(const std::string &path, const std::string &fname, std::ifstream &file)
//------------------------------------------------------------------------------
{
    std::string testPath, remainingPath( grape::removeEndWhiteSpace(path) );
    std::string::size_type loc;

    loc = remainingPath.find(":");
    testPath = remainingPath.substr(0, loc);
    testPath += "/"+fname;

    file.open(testPath.c_str());
    if ( file.is_open() )
    {
        return testPath;
    }

    if(loc == std::string::npos)
    {
        return "";
    }

    remainingPath = remainingPath.substr(loc+1U);
    return openFileFromPath(remainingPath, fname, file);
}

} // grape
