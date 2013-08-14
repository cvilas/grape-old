//==============================================================================
// Project  : Grape
// Module   : utilities
// File     : stringfuncs.h
// Brief    : a few useful string manipulation functions.
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

#ifndef GRAPE_STRINGFUNCS_H
#define GRAPE_STRINGFUNCS_H

#include "grapeutils_common.h"
#include <sstream>
#include <string>
#include <locale>
#include <algorithm>

namespace Grape
{

//------------------------------------------------------------------------------
/// Remove white space characters at the beginning and end of a string
////// \ingroup utils
inline std::string removeEndWhiteSpace(const std::string &s)
//------------------------------------------------------------------------------
{
    std::string str(s);
    static const char sp[] = " \n\t\v\r\f";
    str.erase( 0, str.find_first_not_of(sp) );
    str.erase( str.find_last_not_of(sp) + 1U );
    return str;
}

//------------------------------------------------------------------------------
/// Convert to string representation
/// \ingroup utils
template<class T> std::string TAsString(const T &value)
//------------------------------------------------------------------------------
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

//------------------------------------------------------------------------------
/// Interpret specified string as another type
/// \ingroup utils
template<class T> T stringAsT(const std::string &s)
//------------------------------------------------------------------------------
{
    T t;
    std::istringstream stream(removeEndWhiteSpace(s));
    stream >> t;
    return t;
}

// template specialisation for string to string
template<> inline std::string stringAsT<std::string>(const std::string &s)
{
    return removeEndWhiteSpace(s);
}

// template specialisation for string to bool
template<> inline bool stringAsT<bool>(const std::string &s)
{
    // false: "FALSE", "false", "0", "no"
    // true: anything other than false
    std::string tmp(removeEndWhiteSpace(s));
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

    if( (tmp == "FALSE") || (tmp == "0") || (tmp == "NO") )
    {
        return false;
    }
    else
    {
        return true;
    }
}

} // Grape

#endif // GRAPE_STRINGFUNCS_H
