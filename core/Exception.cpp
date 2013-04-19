//==============================================================================
// Project  : Grape
// Module   : Core
// File     : Exception.cpp
//==============================================================================

#include "Exception.h"


namespace Grape
{
    //--------------------------------------------------------------------------
    Exception::Exception(int code, const char* msg) throw(/*nothing*/)
    //--------------------------------------------------------------------------
    : std::exception()
    { 
        copyFrom(code, msg);
    }
    
    //--------------------------------------------------------------------------
    Exception::Exception(const Exception& e)
    //--------------------------------------------------------------------------
    : std::exception(e)
    {
        copyFrom(e.getCode(), e.what() );
    }
    
    //--------------------------------------------------------------------------
    Exception& Exception::operator=(const Exception& e) throw(/*nothing*/)
    //--------------------------------------------------------------------------
    {
        if( this != &e )
        {
            copyFrom( e.getCode(), e.what() );
        }
        return *this;
    }
    
    //--------------------------------------------------------------------------
    void Exception::copyFrom(int code, const char* msg) throw(/*nothing*/)
    //--------------------------------------------------------------------------
    {
        _code = code;
        size_t len = strlen(msg);
        if( len > (MAX_MSG_LEN-1) )
        {
            len = MAX_MSG_LEN-1;
        }
        strncpy(_msgStr, msg, len);
        _msgStr[len] = '\0';
    }
    
} // Generic


