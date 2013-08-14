//==============================================================================
// Project  : Grape
// Module   : IO
// File     : IJoystick.h
// Brief    : Joystick interface
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

#ifndef GRAPEIO_IJOYSTICK_H
#define GRAPEIO_IJOYSTICK_H

#include "grapeio_common.h"
#include <vector>
#include <string>

namespace Grape
{

/// \class IJoystick
/// \ingroup io
/// \brief Interface definition for joysticks
///
/// IJoystick provides a generic interface to joysticks and gamepads.
/// To use a joystick in your application, call the class methods in
/// the following order:
/// - connect() to establish connection with the device.
/// - update() to refresh state of all buttons and axes
/// - getState() to read state.
/// - disconnect() when done with the device.

class GRAPEIO_DLL_API IJoystick
{
public:

    /// \brief Joystick state
    class JoystickState
    {
    public:
        unsigned long long  ms;         //!< Timestamp in milliseconds.
        std::vector<int>	axes;		//!< Analogue axes. range: (-32768 - +32767)
        std::vector<int> 	pov;		//!< POV directions. (-1, 0 - 360000)
        std::vector<int> 	buttons;	//!< Buttons. (1 = pressed)
    }; // JoystickState

protected:
    IJoystick() {}

public:
    virtual ~IJoystick() {}

    virtual bool connect(const std::string& port) = 0;  //!< Connect. Return true on success
    virtual void disconnect() = 0;                      //!< Disconnect from joystick
    virtual bool isConnected() const = 0;               //!< \return true if joystick is connected and active
    virtual std::string getName() const = 0;            //!< \return name of connected device
    virtual bool setDeadZone(int val) = 0;              //!< Set dead zone for all analog axes. (range 0 - 32767)
    virtual int getDeadZone() const = 0;                //!< \return current dead zone setting for all analog axes. \see setDeadZone.
    virtual bool update() = 0;                          //!< Read and update state. \return true on success. \see getState.
    const JoystickState& getState() const { return _state; } //!< Reference to state data. The data is updated after every call to update. \see update.
protected:
    JoystickState _state;                               //!< joystick state object

private:
    IJoystick(const IJoystick &);                       //!< private to disable copy
    IJoystick &operator=(const IJoystick &);            //!< private to disable assignment
}; // IJoystick

} // Grape
#endif // GRAPEIO_IJOYSTICK_H
