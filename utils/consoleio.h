//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : consoleio.h
// Brief    : console I/O functions
//==============================================================================

#ifndef GRAPE_CONSOLEIO_H
#define	GRAPE_CONSOLEIO_H

#include "grapeutils_common.h"

namespace Grape
{

/// Read a single character from console without echoing.
/// \ingroup utils
int GRAPEUTILS_DLL_API getch();

/// Check console for a recent keystroke.
/// \ingroup utils
/// \return nonzero if key was pressed. use getch() to read the key stroke.
int GRAPEUTILS_DLL_API kbhit();

} // Grape

#endif	// GRAPE_CONSOLEIO_H

