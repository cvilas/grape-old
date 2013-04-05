//==============================================================================
// Project  : Grape
// Module   : Core
// File     : grape_common.h
// Brief    : common header
//==============================================================================

#ifndef GRAPE_COMMON_H
#define	GRAPE_COMMON_H

// Windows only:
// Define GRAPE_DLL and GRAPE_DLL_EXPORT when creating Grape libraries
// Define only GRAPE_DLL when linking against Grape libraries.
#ifdef GRAPE_DLL
#	ifdef GRAPE_DLL_EXPORT
#		define GRAPE_DLL_API __declspec(dllexport)
#	else
#		define GRAPE_DLL_API __declspec(dllimport)
#	endif
#else
#	define GRAPE_DLL_API
#endif

namespace Grape
{

// All Grape classes are contained in this namespace

/// Sleep for specified milliseconds
/// \ingroup common
GRAPE_DLL_API void milliSleep(unsigned int ms);

} // Grape

#endif // GRAPE_COMMON_H
