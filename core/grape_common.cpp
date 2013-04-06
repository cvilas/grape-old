//==============================================================================
// Project  : Grape
// Module   : Core
// File     : grape_common.cpp
//==============================================================================

#include "grape_common.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace Grape
{

void milliSleep(unsigned int ms)
{

#ifdef _WIN32
    Sleep(ms);
#else
    usleep(1000*ms);
#endif

}

} // Grape


