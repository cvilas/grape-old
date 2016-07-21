//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SimpleJoystick.h
// Brief    : Simple joystick
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

#ifndef GRAPEIO_SIMPLEJOYSTICK_H
#define GRAPEIO_SIMPLEJOYSTICK_H

#include "IJoystick.h"
#ifdef _MSC_VER
#include <Windows.h>
#else
#define HWND void*
#endif

namespace  grape
{

/// \class SimpleJoystick
/// \ingroup io
/// \brief Generic interface to OS supported joysticks
/// \todo
/// - Reliable detection of physical disconnection
/// - Windows: [bug] joystick initial state incorrect until movement occurs
/// - Event handler interface to notify state changes
///   See http://msdn.microsoft.com/en-us/library/windows/desktop/ee418748%28v=vs.85%29.aspx
/// - Calibration functions and I/O curves for axes
/// - Update system for new DirectX (part of Windows 8 SDK)
///
/// <b>Example Program</b>
/// \include SimpleJoystickExample.cpp

class GRAPEIO_DLL_API SimpleJoystick : public IJoystick
{
public:
    /// Constructor.
    /// \param hWnd (input) This parameter is required for Windows but ignored
    ///             otherwise. In Windows, this is the 'top level' window handle
    ///             for the application. For a console application, the handle
    ///             can be obtained as follows:
    ///             \code
    ///             // window handle
    ///             TCHAR szConsoleTitle[MAX_PATH];
    ///             GetConsoleTitle(szConsoleTitle, MAX_PATH);
    ///             HWND hWnd = FindWindow(NULL, szConsoleTitle);
    ///             \endcode
    ///             You may also be able to get away with the following:
    ///             \code
    ///             HWND hWnd = GetForegroundWindow();
    ///             if( hWnd == NULL )hWnd = GetDesktopWindow();
    ///             \endcode
    SimpleJoystick(HWND hWnd);
    ~SimpleJoystick();

    /// Connect to device.
    /// \param port (input) The joystick device.
    ///         In Linux, this is the path to device, e.g. "/dev/input/js0".
    ///         In Windows, this can be any of the following:
    ///         - device index as a string formatted integer (eg: "0")
    ///         - device name string (eg: "Microsoft XBox 360 Receiver"
    ///         - device GUID string (eg: "73d0ee30-f392-11dd-8001-444553540000")
    ///         Use methods in Dx8JoystickManager class to determine the
    ///         indices, names and GUIDs of connected joysticks.
    /// \return true on success.
    bool connect(const std::string& port);
    void disconnect();

    /// Check whether device is connected.
    /// Note: This doesn't work well under linux. A solution is to call
    /// disconnect() if update() fails, and attempt a reconnection by
    /// calling connect() again.
    /// \return true if connection to the device is active.
    bool isConnected() const;

    std::string getName() const;
    bool setDeadZone(int val);
    int getDeadZone() const;
    bool update();
private:
    class SimpleJoystickP* _pImpl; //!< platform specific private implementation
};

} // grape

#endif // GRAPEIO_SIMPLEJOYSTICK_H
