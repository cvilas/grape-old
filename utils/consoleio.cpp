//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : consoleio.cpp
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

#include "consoleio.h"
#include <stdio.h>
#ifdef _MSC_VER
#include <conio.h>
#else
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#endif


namespace grape
{

//==============================================================================
int getch()
//==============================================================================
{
#ifdef _MSC_VER
    return _getch();
#else
    // from http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDERR_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr( STDERR_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return ch;
#endif
}


//==============================================================================
int kbhit()
//==============================================================================
{
#ifdef _MSC_VER
    return _kbhit();
#else
    // from http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
    struct timeval tv;
    fd_set readFd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&readFd);
    FD_SET(STDIN_FILENO, &readFd);
    if(select(STDIN_FILENO+1, &readFd, NULL, NULL, &tv) == -1)
    {
        return 0;  // error
    }

    if(FD_ISSET(STDIN_FILENO, &readFd))
    {
        return 1;
    }

    return 0;
#endif
}

} // Generic

