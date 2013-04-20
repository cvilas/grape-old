//==============================================================================
// Project  : Grape
// Module   : Utilities
// File     : consoleio.cpp
//==============================================================================

#include "consoleio.h"
#include <stdio.h>
#ifdef _MSC_VER
#include <conio.h>
#else
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#endif


namespace Grape
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

