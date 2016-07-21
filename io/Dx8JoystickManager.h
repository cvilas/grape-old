//==============================================================================
// Project  : Grape
// Module   : IO
// File     : Dx8JoystickManager.h
// Brief    : Manager for DirectX 8.0 supported joysticks
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

#ifndef GRAPEIO_DX8JOYSTICKMANAGER_H
#define GRAPEIO_DX8JOYSTICKMANAGER_H

#define DIRECTINPUT_VERSION 0x0800 // Must be declared before dinput.h

#include "grapeio_common.h"
#include "utils/guidfuncs.h"
#include <dinput.h>
#include <vector>
#include <string>

namespace grape
{

/// \class Dx8JoystickManager
/// \brief Microsoft DirectInput interface manager for joysticks
///
/// Dx8JoystickManager provides convenience functions to manage joysticks using
/// the DirectInput interface. Use this class to get information about
/// attached devices on Windows machines directly. SimpleJoystick uses methods
/// from this class to provide its Windows implementation. This class is not
/// thread safe.
/// User applications must link against dxguid.lib dxerr.lib dinput8.lib comctl32.lib
/// (DirectX SDK or runtimes can be installed
/// from http://msdn.microsoft.com/en-us/directx/aa937788.aspx)
///
/// <b>Example Program</b>
/// \include Dx8JoystickManagerExample.cpp

class GRAPEIO_DLL_API Dx8JoystickManager
{
public:
    /// Collect a list of attached devices.
    /// \return true on success
    static bool enumerateDevices();

    /// \return Number of devices found after last enumeration.
    inline static unsigned int getNumDevices();

    /// Get a device instance from list of enumerated devices.
    /// \param index    (input) 0-based index into the enumerated device list.
    /// \param dinst    (output) On successful return, contains a populated device
    ///                 instance data structure, otherwise undefined
    /// \return         True on success.
    static bool getDeviceInstance(unsigned int index, DIDEVICEINSTANCE &dinst);

    /// Get the name of a device
    /// \param index    (input) 0-based index into the enumerated device list.
    /// \return         Name string on succes, else an empty string.
    static std::string getDeviceName(unsigned int index);

    /// Get a globally unique device identifier for an enumerated device.
    /// The returned ID is unique to a particular computer. An application can
    /// save the ID into a configuration file for later use, such as
    /// reconnection to a preferred attached device.
    /// \param index    (input) 0-based index into the enumerated device list.
    /// \return         A globally unique identifier, or GUID_NULL if error.
    static const GUID &getDeviceGuid(unsigned int index);

    /// Initialize/Re-initialize connection to a Joystick device by index.
    /// \param index (input) 0-based index into the enumerated device list.
    /// \param guid  (output) The instance GUID of the connected device.
    /// \return      valid pointer on success, else NULL.
    static LPDIRECTINPUTDEVICE8 attach(unsigned int index, GUID &guid);

    /// Initialize/Re-initialize connection to a Joystick device by name. If
    /// more than one device has the same name, connection is attempted for
    /// the first available joystick with the given name.
    /// \param name (input) A device instance name.
    /// \param guid (output) The instance GUID of the connected device.
    /// \return     valid pointer on success, else NULL.
    static LPDIRECTINPUTDEVICE8 attach(const std::string &name, GUID &guid);

    /// Connect to a device by its globally unique identifier.
    /// \param guid GUID for the instance of a specific device. See getDeviceGuid().
    /// \return     valid pointer on success, else NULL.
    static LPDIRECTINPUTDEVICE8 attach(const GUID& guid);

    /// Detach a connected device.
    static void detach(LPDIRECTINPUTDEVICE8 &pDevice);

    /// Check if a device is plugged in. Note that this isn't necessarily the
    /// device we are attached to (see attach()).
    /// \param guid GUID of the device.
    /// \return True if the device is plugged in, false if disconnected or unknown.
    inline static bool isDeviceConnected(const GUID &guid);

    /// \return Reference to single me
    inline static Dx8JoystickManager& getSingleton();

private:
    Dx8JoystickManager();
    ~Dx8JoystickManager();
    Dx8JoystickManager(const Dx8JoystickManager&); //!< disable copy
    Dx8JoystickManager& operator=(const Dx8JoystickManager&); //!< disable assignment

    /// Device enumeration callback. Adds any device found to the global devices list.
    static BOOL CALLBACK enumDeviceCallBack(const DIDEVICEINSTANCE* lpddi, void* ptr);

private:
    LPDIRECTINPUT8					_pDirectInput; //!< DirectInput interface
    std::vector<DIDEVICEINSTANCE>	_deviceList; //!< List of enumerated devices
}; // Dx8JoystickManager

//------------------------------------------------------------------------------
Dx8JoystickManager& Dx8JoystickManager::getSingleton()
//------------------------------------------------------------------------------
{
    static Dx8JoystickManager s_singleton;
    return s_singleton;
}

//------------------------------------------------------------------------------
unsigned int Dx8JoystickManager::getNumDevices()
//------------------------------------------------------------------------------
{
    return (unsigned int)(getSingleton()._deviceList.size());
}

//------------------------------------------------------------------------------
bool Dx8JoystickManager::isDeviceConnected(const GUID& guid)
//------------------------------------------------------------------------------
{
    LPDIRECTINPUT8 pDI = getSingleton()._pDirectInput;

    if( pDI )
    {
        return ( pDI->GetDeviceStatus(guid) == S_OK);
    }
    return false;
}

} // grape

#endif // GRAPEIO_DX8JOYSTICKMANAGER_H
