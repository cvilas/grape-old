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


} // Grape
