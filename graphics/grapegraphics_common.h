//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : grapegraphics_common.h
// Brief    : common header for graphics module
//==============================================================================

#ifndef GRAPEGRAPHICS_COMMON_H
#define GRAPEGRAPHICS_COMMON_H

#include <qglobal.h>

#if QT_VERSION < 0x040000
#	error Qt Version 4.0 and above is required
#endif

// Windows only:
// Define GRAPE_..._DLL and GRAPE_..._DLL_EXPORT when creating Grape libraries
// Define only GRAPE_..._DLL when linking against Grape libraries.
#ifdef GRAPEGRAPHICS_DLL
#	ifdef GRAPEGRAPHICS_DLL_EXPORT
#		define GRAPEGRAPHICS_DLL_API __declspec(dllexport)
#	else
#		define GRAPEGRAPHICS_DLL_API __declspec(dllimport)
#	endif
#else
#	define GRAPEGRAPHICS_DLL_API
#endif

#ifdef _MSC_VER // using microsoft visual studio
#pragma warning( disable : 4290 ) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable : 4251 ) // class X needs to have dll-interface to be used by clients of class Y
#endif

namespace Grape
{


} // Grape

#endif // GRAPEGRAPHICS_COMMON_H
