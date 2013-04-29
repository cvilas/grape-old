//==============================================================================
// Project  : Grape
// Module   : utilities
// File     : guidfuncs.cpp
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
