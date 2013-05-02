//==============================================================================
// Project  : Grape
// Module   : utilities
// File     : guidfuncs.h
// Brief    : a few useful GUID manipulation functions.
//==============================================================================

#ifndef GRAPEUTILS_GUIDFUNCS_H
#define GRAPEUTILS_GUIDFUNCS_H

#include "grapeutils_common.h"
#include <InitGuid.h>
#include <string>
#include <rpc.h>

#ifndef GUID_NULL
const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
#endif

namespace Grape
{

std::string GRAPEUTILS_DLL_API GuidToString(GUID& guid);
bool GRAPEUTILS_DLL_API stringToGuid(const std::string &str, GUID& guid);
bool GRAPEUTILS_DLL_API isGuidEqual(const GUID& id1, const GUID& id2);

} // Grape

#endif // GRAPEUTILS_GUIDFUNCS_H
