//==============================================================================
// File     :   JoystickExamples.cpp
// Brief    :   Example programs for Joystick library
//==============================================================================

#include "JoystickExamples.h"
#include "utils/consoleio.h"
#include <iostream>

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    argc = argc;
    argv = argv;

    int nExamples = 0;
    int nPassed = 0;

    nExamples++;
    std::cout << std::endl << "SimpleJoystickExample" << std::endl;
    if( SimpleJoystickExample() ) { nPassed++; }
    else { std::cout << "FAILED" << std::endl; }

#ifdef _MSC_VER
    nExamples++;
    std::cout << std::endl << "Dx8JoystickManagerExample" << std::endl;
    if( Dx8JoystickManagerExample() ) { nPassed++; }
    else { std::cout << "FAILED" << std::endl; }
#endif

    std::cout << "Passed " << nPassed << "/" << nExamples << " examples." << std::endl;
	
	std::cout << "Press any key to exit.." << std::endl;
    Grape::getch();

    return 0;

}

