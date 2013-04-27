//==============================================================================
/// \file        Dx8JoystickManagerExample.cpp
/// \brief       Example program for Dx8JoystickManager class
/// \author      vilasc
/// \date        3 July 2012 (created)
///
/// <h3>Last Modified details:</h3>
/// $Author: vilas.chitrakaran $
/// $Date: 2012-07-11 17:03:19 +0100 (Wed, 11 Jul 2012) $
/// $Revision: 12405 $
//==============================================================================

#include "Dx8JoystickManager.h"
#include <iostream>

//==============================================================================
bool Dx8JoystickManagerExample()
//==============================================================================
{
    Grape::Dx8JoystickManager& manager = Grape::Dx8JoystickManager::getSingleton();

    // collect the list of attached devices
    if( manager.enumerateDevices() == false )
    {
        std::cout << "Enumeration failed" << std::endl;
        return false;
    }

    unsigned int nDevices = manager.getNumDevices();
    if( nDevices == 0 )
    {
        std::cout << "No connected joysticks" << std::endl;
        return true;
    }

    // list devices
    for( unsigned int i = 0; i < nDevices; ++i )
    {
        GUID gid = manager.getDeviceGuid(i);
        std::cout << "Index: " << i << std::endl
                  << "\tName: " << manager.getDeviceName(i) << std::endl
                  << "\tGUID: " << Joystick::GuidToString(gid) << std::endl;
    }

    return true;
}


