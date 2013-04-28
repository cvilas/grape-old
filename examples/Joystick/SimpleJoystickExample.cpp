//==============================================================================
// File     :   SimpleJoystickExample.cpp
// Brief    :   Example program for SimpleJoystick class
//==============================================================================

#include "JoystickExamples.h"
#include "io/SimpleJoystick.h"
#include "utils/consoleio.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>

//==============================================================================
bool SimpleJoystickExample()
//==============================================================================
{
    HWND hWnd = 0;

#ifdef _MSC_VER
    const std::string DEVICE = "0";
	TCHAR szConsoleTitle[MAX_PATH];
	GetConsoleTitle(szConsoleTitle, MAX_PATH);
	Sleep(40);
    hWnd = FindWindow(NULL, szConsoleTitle);
    if( hWnd == NULL )
    {
        std::cerr << "Can't get console window handle" << std::endl;
        return false;
    }
#else
    const std::string DEVICE = "/dev/input/js1";
#endif

    Grape::SimpleJoystick* pJoystick = NULL;

    pJoystick = new Grape::SimpleJoystick(hWnd);
    if( !pJoystick )
    {
        std::cerr << "Unable to create joystick" << std::endl;
        return false;
    }

	// connect to first joystick.
    if( !pJoystick->connect(DEVICE) )
    {
        std::cerr << "Error connecting to joystick" << std::endl;
        delete pJoystick;
        return false;
    }

    // read joystick
    std::cout << "Reading joystick " << pJoystick->getName() << std::endl;
    std::cout << "Press any key to exit" << std::endl;

    pJoystick->setDeadZone(3000);
    std::cout << "Axes dead zone " << pJoystick->getDeadZone() << std::endl;

    while( !Grape::kbhit() )
    {
		// if we lost connection, try again
        if( !pJoystick->isConnected() )
        {
            std::cout << "Device connection lost. Trying again.." << std::endl;
            pJoystick->connect(DEVICE);
            continue;
        }

		// refresh 
        if( !pJoystick->update() )
        {
            std::cout << "Device update failed" << std::endl;
            pJoystick->disconnect(); // will try to connect again.
            continue;
        }

		// print
        const Grape::SimpleJoystick::JoystickState state = pJoystick->getState();
        std::cout << "[" << state.ms << "] Axes: ";
        for( unsigned int i = 0; i < state.axes.size(); ++i )
        {
            std::cout << state.axes[i] << " ";
        }
        std::cout << " Buttons: ";
        for(unsigned int i = 0; i < state.buttons.size(); ++i)
        {
            if( state.buttons[i] )
                std::cout << "|";
            else
                std::cout << ".";
        }
        std::cout << " POV: ";
        for(unsigned int i = 0; i < state.pov.size(); ++i)
        {
            std::cout << state.pov[i] << '\t';
        }
        std::cout << '\r' << std::flush;
    }

    // cleanup and exit
    pJoystick->disconnect();
    delete pJoystick;
    std::cout << "\nKey " << Grape::getch() << " pressed. Exiting" << std::endl;

    return true;
}
