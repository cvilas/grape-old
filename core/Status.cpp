//==============================================================================
// Project  : Grape
// Module   : Core
// File     : StatusMessage.cpp
//==============================================================================

#include "Status.h"

namespace Grape
{

//=============================================================================
std::ostringstream& Status::set(long long int code)
//=============================================================================
{
    _code = code;
    _stream.clear();
    _stream.str(std::string());
    return _stream;
}

} // Grape
