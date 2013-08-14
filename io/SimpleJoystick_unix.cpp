//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SimpleJoystick_unix.cpp
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

#include "SimpleJoystick.h"
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include <iostream>
#include <memory.h> // for memset
#include <errno.h>
#include <cmath>
#include <sstream>

namespace Grape
{

/// \class SimpleJoystickP
/// \brief Linux specific private implementation.
///
/// The following implementation uses the linux joystick api. See
/// www.kernel.org/doc/Documentation/input/joystick-api.txt
class SimpleJoystickP
{
public:
    SimpleJoystickP();
    ~SimpleJoystickP() {}

    bool open(const std::string& path);
    void close();
    int getFd() const { return _fd; }
    bool setNonBlocking(bool dontBlock);
    void processEvent(const struct js_event& jsev, SimpleJoystick::JoystickState& state);
public:
    int         _fd;                            //!< device file descriptor
    std::string _idStr;                         //!< identifier string
    int         _deadZone;
};

//==============================================================================
SimpleJoystickP::SimpleJoystickP()
//==============================================================================
    : _fd(-1),
      _idStr("uninitialised"),
      _deadZone(0)
{}

//------------------------------------------------------------------------------
bool SimpleJoystickP::open(const std::string& path)
//------------------------------------------------------------------------------
{
    return ( -1 != (_fd = ::open(path.c_str(), O_RDONLY)) );
}

//------------------------------------------------------------------------------
void SimpleJoystickP::close()
//------------------------------------------------------------------------------
{
    ::close(_fd);
    _fd = -1;
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::setNonBlocking(bool dontBlock)
//------------------------------------------------------------------------------
{
    int flags = fcntl(_fd, F_GETFL);
    if( flags == -1 )
    {
        return false;
    }

    bool isNonBlocking = (flags & O_NONBLOCK);

    if( dontBlock != isNonBlocking )
    {
        ( dontBlock ) ? (flags |= O_NONBLOCK) : (flags &= ~O_NONBLOCK);
        return fcntl(_fd, F_SETFL, flags) != -1;
    }

    return true;
}

//------------------------------------------------------------------------------
void SimpleJoystickP::processEvent(const struct js_event& jsev, SimpleJoystick::JoystickState& state)
//------------------------------------------------------------------------------
{
    state.ms = jsev.time;

    switch( jsev.type & ~JS_EVENT_INIT )
    {
    case JS_EVENT_AXIS:
        if( jsev.number < state.axes.size() )
        {
            if( std::abs(jsev.value) > _deadZone)
            {
                state.axes[jsev.number] = jsev.value;
            }
            else
            {
                state.axes[jsev.number] = 0;
            }
        }
        //std::cout << jsev.time << " AXIS " << (int)jsev.number << " " << jsev.value << std::endl;
        break;
    case JS_EVENT_BUTTON:
        if( jsev.number < state.buttons.size() )
        {
            state.buttons[jsev.number] = jsev.value;
        }
        //std::cout << state.ms << " BUTT " << (int)jsev.number << " " << jsev.value << std::endl;
        break;
    default:
        break;
    }
}

//==============================================================================
SimpleJoystick::SimpleJoystick(HWND hWnd)
//==============================================================================
    : _pImpl(new SimpleJoystickP)
{
    hWnd = hWnd; //unused
}

//------------------------------------------------------------------------------
SimpleJoystick::~SimpleJoystick()
//------------------------------------------------------------------------------
{
    disconnect();
    delete _pImpl;
}

//------------------------------------------------------------------------------
bool SimpleJoystick::connect(const std::string& path)
//------------------------------------------------------------------------------
{
    disconnect();

    // attempt a connection
    if( !_pImpl->open(path) )
    {
        return false;
    }

    // device caps
    int numAxes = 0;
    int numButtons = 0;
    char name[128];
    memset(name, 0, 128);
    ioctl( _pImpl->_fd, JSIOCGAXES, &numAxes );
    ioctl( _pImpl->_fd, JSIOCGBUTTONS, &numButtons );
    ioctl( _pImpl->_fd, JSIOCGNAME(sizeof(name)), &name );

    _pImpl->_idStr = name;
    _state.axes.resize(numAxes, 0);
    _state.buttons.resize(numButtons, 0);
    _state.pov.resize(0);

#ifdef _DEBUG
    std::cout << "[SimpleJoystick::connect] Opened device " << path << std::endl
              << "\tName: " << _pImpl->_idStr << std::endl
              << "\tNumber of axes: " << numAxes << std::endl
              << "\tNumber of buttons: " << numButtons << std::endl;
#endif

    // we will be polling the device
    // Note: if you ever want to wait until a joystick event,
    // call setNonBlocking(false) instead.
    _pImpl->setNonBlocking(true);

    return true;
}

//------------------------------------------------------------------------------
void SimpleJoystick::disconnect()
//------------------------------------------------------------------------------
{
    _pImpl->close();

    _state.axes.clear();
    _state.buttons.clear();
    _state.pov.clear();
}

//------------------------------------------------------------------------------
bool SimpleJoystick::isConnected() const
//------------------------------------------------------------------------------
{
    /// \todo
    /// need a better way...
    return (fcntl(_pImpl->_fd, F_GETFL) != -1) || (errno != EBADF);
}

//------------------------------------------------------------------------------
std::string SimpleJoystick::getName() const
//------------------------------------------------------------------------------
{
    return _pImpl->_idStr;
}

//------------------------------------------------------------------------------
bool SimpleJoystick::setDeadZone(int val)
//------------------------------------------------------------------------------
{
    _pImpl->_deadZone = std::abs(val);
    return true;
}

//------------------------------------------------------------------------------
int SimpleJoystick::getDeadZone() const
//------------------------------------------------------------------------------
{
    return _pImpl->_deadZone;
}

//------------------------------------------------------------------------------
bool SimpleJoystick::update()
//------------------------------------------------------------------------------
{
    //_pImpl->setNonBlocking(poll);

    // read as many reports as possible
    struct js_event ev[0xff];
    int bytesRead = ::read(_pImpl->_fd, ev, sizeof(ev));
    if( bytesRead < 0 )
    {
        return (errno == EAGAIN);
    }

    // Note: The linux joystick driver queues up to JS_BUFF_SIZE events.
    // JS_BUFF_SIZE is typically 64, so our buffer size of 0xff should be
    // sufficient. If not, do nonblocking read() until queue is empty (read
    // returns -1 and errno EAGAIN is set)

    // process
    int sz = sizeof(struct js_event);
    int nEv = bytesRead/sz;
    for( int i = 0; i < nEv; ++i )
    {
        _pImpl->processEvent(ev[i], _state);
    }

    return true;
}

} // Grape
