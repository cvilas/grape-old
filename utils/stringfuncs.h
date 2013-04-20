//==============================================================================
// Project  : Grape
// Module   : utilities
// File     : stringfuncs.h
// Brief    : a few useful string manipulation functions.
//==============================================================================

#ifndef GRAPE_STRINGFUNCS_H
#define GRAPE_STRINGFUNCS_H

#include "grapeutils_common.h"
#include <sstream>
#include <string>
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
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int))std::toupper);
    // typecasting toupper() removes ambiguity for same function defined
    // in <locale> and <cctype>. We want to use <cctype> version

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
