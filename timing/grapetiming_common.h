//==============================================================================
// Project  : Grape
// Module   : Timing
// File     : grapetiming_common.h
// Brief    : common header for timing module
//==============================================================================

#ifndef GRAPETIMING_COMMON_H
#define	GRAPETIMING_COMMON_H

// Windows only:
// Define GRAPE_..._DLL and GRAPE_..._DLL_EXPORT when creating Grape libraries
// Define only GRAPE_..._DLL when linking against Grape libraries.
#ifdef GRAPETIMING_DLL
#	ifdef GRAPETIMING_DLL_EXPORT
#		define GRAPETIMING_DLL_API __declspec(dllexport)
#	else
#		define GRAPETIMING_DLL_API __declspec(dllimport)
#	endif
#else
#	define GRAPETIMING_DLL_API
#endif

#ifdef _MSC_VER // using microsoft visual studio
#pragma warning( disable : 4290 ) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable : 4251 ) // class X needs to have dll-interface to be used by clients of class Y
#endif

namespace Grape
{

// All Grape classes are contained in this namespace

/// Sleep for specified milliseconds
/// \ingroup timing
GRAPETIMING_DLL_API void milliSleep(unsigned int ms);

} // Grape


#endif // GRAPETIMING_COMMON_H
