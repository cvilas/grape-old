//==============================================================================
// File    :   Dx8JoystickManagerExample.cpp
// Brief   :   Example program for Dx8JoystickManager class
//==============================================================================

#include "io/Dx8JoystickManager.h"
#include <iostream>

//==============================================================================
int main()
//==============================================================================
{
    grape::Dx8JoystickManager& manager = grape::Dx8JoystickManager::getSingleton();

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
                  << "\tGUID: " << grape::GuidToString(gid) << std::endl;
    }

    return true;
}


