//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : ConfigFile.cpp
//==============================================================================

#include "ConfigFile.h"
#include <fstream>
#include <time.h>

namespace Grape
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
ConfigNode* ConfigFile::getConfig(const std::string& path)
//------------------------------------------------------------------------------
{
    std::string remainingPath( Grape::removeEndWhiteSpace(path) );
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
std::string ConfigFile::openFileFromPath(const std::string &path, const std::string &fname, std::ifstream &file)
//------------------------------------------------------------------------------
{
    std::string testPath, remainingPath( Grape::removeEndWhiteSpace(path) );
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

} // Grape
