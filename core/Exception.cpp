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
    Exception::Exception(const Exception& e) throw(/*nothing*/)
    //--------------------------------------------------------------------------
    : std::exception(e)
    {
        copyFrom(e.code(), e.what() );
    }
    
    //--------------------------------------------------------------------------
    Exception& Exception::operator=(const Exception& e) throw(/*nothing*/)
    //--------------------------------------------------------------------------
    {
        if( this != &e )
        {
            copyFrom( e.code(), e.what() );
        }
        return *this;
    }
    
    //--------------------------------------------------------------------------
    void Exception::copyFrom(int code, const char* msg) throw(/*nothing*/)
    //--------------------------------------------------------------------------
    {
        _code = code;
        size_t len = strlen(msg);
        if( len > (MAX_MESSAGE_LENGTH-1) )
        {
            len = MAX_MESSAGE_LENGTH-1;
        }
        strncpy(_msgStr, msg, len);
        _msgStr[len] = '\0';
    }
    
} // Generic


