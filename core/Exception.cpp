//==============================================================================
// Project  : Grape
// Module   : Core
// File     : Exception.cpp
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

#include "Exception.h"


namespace grape
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


