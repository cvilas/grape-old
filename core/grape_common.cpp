//==============================================================================
// Project  : Grape
// Module   : Core
// File     : grape_common.cpp
//==============================================================================

#include "grape_common.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Grape
{

void milliSleep(unsigned int ms)
{

#ifdef _WIN32
	Sleep(1);
#else
	usleep(1000);
#endif

}

} // Grape


