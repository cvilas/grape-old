//==============================================================================
// Project  : Grape
// Module   : utilities
// File     : grapeutils_common.h
// Brief    : common header for utilities module
//==============================================================================

#ifndef GRAPEUTILS_COMMON_H
#define	GRAPEUTILS_COMMON_H

// Windows only:
// Define GRAPE_..._DLL and GRAPE_..._DLL_EXPORT when creating Grape libraries
// Define only GRAPE_..._DLL when linking against Grape libraries.
#ifdef GRAPEUTILS_DLL
#	ifdef GRAPEUTILS_DLL_EXPORT
#		define GRAPEUTILS_DLL_API __declspec(dllexport)
#	else
#		define GRAPEUTILS_DLL_API __declspec(dllimport)
#	endif
#else
#	define GRAPEUTILS_DLL_API
#endif

#ifdef _MSC_VER // using microsoft visual studio
#pragma warning( disable : 4290 ) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable : 4251 ) // class X needs to have dll-interface to be used by clients of class Y
#endif

namespace Grape
{

// All Grape classes are contained in this namespace


} // Grape

#endif // GRAPEUTILS_COMMON_H
