//==============================================================================
// Project  : Grape
// Module   : utilities
// File     : guidfuncs.cpp
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

#include "guidfuncs.h"

namespace Grape
{

//-------------------------------------------------------------------------
std::string GuidToString(GUID& guid)
//-------------------------------------------------------------------------
{
    std::string str;
    WCHAR* wcharStr = 0;
    if( UuidToString(&guid, (RPC_WSTR*)(&wcharStr)) == RPC_S_OK )
    {
        std::wstring wstr(wcharStr);
        str = std::string(wstr.begin(), wstr.end());
        RpcStringFree((RPC_WSTR*)(&wcharStr));
    }
    return str;
}

//-------------------------------------------------------------------------
bool stringToGuid(const std::string &str, GUID& guid)
//-------------------------------------------------------------------------
{
    std::wstring wstr = std::wstring(str.begin(), str.end());
    return ( UuidFromString( (RPC_WSTR)(wstr.c_str()), &guid) == RPC_S_OK );
}

//-------------------------------------------------------------------------
bool isGuidEqual(const GUID &id1, const GUID &id2 )
//-------------------------------------------------------------------------
{
    if( id1.Data1 != id2.Data1 )
    {
        return false;
    }
    else if( id1.Data2 != id2.Data2 )
    {
        return false;
    }
    else if( id1.Data3 != id2.Data3 )
    {
        return false;
    }
    __int64* pData41 = (__int64*)(id1.Data4);
    __int64* pData42 = (__int64*)(id2.Data4);
    if( *pData41 != *pData42 )
    {
        return false;
    }
    return true;
}

} // Grape
